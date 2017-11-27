// Toy DBMS, v0.4
// George Chernishev (c) 2016-2017, chernishev<at>google mail
// A task framework for undergraduate students at Saint-Petersburg Academic University, DBMS development course
// More details regarding the course can be found here: www.math.spbu.ru/user/chernishev/
// CHANGELOG:
// 0.4: no chance for efficiency competition, so, this year I reoriented task towards ideas:
//      1) tried to remove C code, now we are using lots of std:: stuff
//      2) implemented support for multiple attributes in the DBMS
//      3) code clean-up and restructurization
// 0.3: added:
//      1) support for restricting physical join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include <algorithm>
#include <tuple>
#include <string.h>
#include <sys/time.h>
#include "interface.h"

LAbstractNode::LAbstractNode(LAbstractNode* left, LAbstractNode* right, int block_size){
    this->left = left;
    this->right = right;
    this->block_size = block_size;
}

LAbstractNode::~LAbstractNode(){
}

LAbstractNode* LAbstractNode::GetLeft(){
    return left;
}

LAbstractNode* LAbstractNode::GetRight(){
    return right;
}

LJoinNode::LJoinNode(
        LAbstractNode* left, // assumed to be not null
        LAbstractNode* right,
        std::string offset1,
        std::string offset2,
        int memorylimit
    ): LAbstractNode(left, right, left->get_block_size()) {

    this->attr1 = offset1;
    this->attr2 = offset2;
    this->memorylimit = memorylimit;

    std::vector<std::string> match;
    ValueType vt;
    COLUMN_SORT cs;
    for (int i = 0; i < left->fieldNames.size(); i++){
        for (int j = 0; j < right->fieldNames.size(); j++){
            if((left->contains_str(i, offset1)
                && right->contains_str(j, offset2))
                || (left->contains_str(i, offset2)
                && right->contains_str(j, offset1))) {
                match = left->fieldNames[i];
                match.insert(std::end(match),
                             std::begin(right->fieldNames[j]),
                             std::end(right->fieldNames[j]));
                vt = left->fieldTypes[i];
                cs = left->fieldOrders[i];
            }
        }
    }

    for (int i = 0; i < left->fieldNames.size(); i++){
        if(!left->contains_str(i, offset1) && !left->contains_str(i, offset2)) {
            fieldNames.push_back(left->fieldNames[i]);
            fieldTypes.push_back(left->fieldTypes[i]);
            fieldOrders.push_back(left->fieldOrders[i]);
        }
    }

    for (int i = 0; i < right->fieldNames.size(); i++){
        if(!right->contains_str(i, offset1) && !right->contains_str(i, offset2)) {
            fieldNames.push_back(right->fieldNames[i]);
            fieldTypes.push_back(right->fieldTypes[i]);
            fieldOrders.push_back(right->fieldOrders[i]);
        }
    }

    fieldNames.push_back(match);
    fieldTypes.push_back(vt);
    fieldOrders.push_back(cs);

}

LJoinNode::~LJoinNode(){
    delete left;
    delete right;
}

LCrossProductNode::LCrossProductNode(
        LAbstractNode* l,
        LAbstractNode* r
    ): LAbstractNode(l, r, l->get_block_size()) {

    fieldNames.insert(std::end(fieldNames),
                      std::begin(left->fieldNames),
                      std::end(left->fieldNames));
    fieldNames.insert(std::end(fieldNames),
                      std::begin(right->fieldNames),
                      std::end(right->fieldNames));
    fieldTypes.insert(std::end(fieldTypes),
                      std::begin(left->fieldTypes),
                      std::end(left->fieldTypes));
    fieldTypes.insert(std::end(fieldTypes),
                      std::begin(right->fieldTypes),
                      std::end(right->fieldTypes));
    fieldOrders.insert(std::end(fieldOrders),
                       std::begin(left->fieldOrders),
                       std::end(left->fieldOrders));
    fieldOrders.insert(std::end(fieldOrders),
                       std::begin(right->fieldOrders),
                       std::end(right->fieldOrders));
}

LCrossProductNode::~LCrossProductNode() {
    delete left;
    delete right;
}

LProjectNode::LProjectNode(
        LAbstractNode* child,
        std::vector<std::string> tokeep
    ): LAbstractNode(child, NULL, child->get_block_size()) {

    for (int i = 0; i < left->fieldNames.size(); i++){
        for (int j = 0; j < tokeep.size(); j++){
            std::vector<std::string> source = left->fieldNames[i];
            std::string candidate = tokeep[j];
            if(std::find(source.begin(), source.end(), candidate) != source.end()){
                fieldNames.push_back(source);
                fieldTypes.push_back(left->fieldTypes[i]);
                fieldOrders.push_back(left->fieldOrders[i]);
                continue;
            }
        }
    }
}

LProjectNode::~LProjectNode(){
  delete left;
}

LSelectNode::LSelectNode(
        BaseTable& table,
        std::vector<Predicate> predicates,
        int block_size
    ): LAbstractNode(NULL, NULL, block_size) {

    this->table = table;
    this->predicates = predicates;
    iteratorpos = 0;
    for (int i = 0; i < table.nbAttr; i++){
        std::string tmp = table.relpath + "." + table.vnames[i];
        std::vector<std::string> tmp2;
        tmp2.push_back(tmp);
        fieldNames.push_back(tmp2);
    }
    fieldTypes = table.vtypes;
    fieldOrders = table.vorders;
}

BaseTable& LSelectNode::GetBaseTable(){
  return table;
}

std::tuple<int, Predicate> LSelectNode::GetNextPredicate(){
  if(predicates.size() == 0 || iteratorpos >= predicates.size()){
      return std::make_tuple(1, Predicate());
  }
  return std::make_tuple(0, predicates[iteratorpos++]);
}

void LSelectNode::ResetIterator(){
  iteratorpos = 0;
}


LSelectNode::~LSelectNode(){
}

LUniqueNode::LUniqueNode(LAbstractNode* child):LAbstractNode(child, NULL, child->get_block_size()){
    fieldNames.insert(std::end(fieldNames),
                      std::begin(child->fieldNames),
                      std::end(child->fieldNames));
    fieldTypes.insert(std::end(fieldTypes),
                      std::begin(child->fieldTypes),
                      std::end(child->fieldTypes));
    fieldOrders.insert(std::end(fieldOrders),
                       std::begin(child->fieldOrders),
                       std::end(child->fieldOrders));
}

LUniqueNode::~LUniqueNode(){
}

//void PResultNode::load_next_block() {
//    std::string line;
//    std::string word;
//    for(int i = 0; i < buffer.size()
//        && getline(data_file, line); ++i) {
//        buffer[i].clear();
//        ++pos;
//        block_pos = (block_pos + 1) % buffer.size();
//        std::istringstream iss(line);
//        for(int j = 0; iss >> word; ++j) {
//            Value h;
//            switch(prototype->field[j]) {
//            case VT_INT:
//                h = Value(std::stoi(word));
//                break;
//            case VT_STRING:
//                h = Value(word);
//                break;
//            }
//            buffer[i].push_back(h);
//        }
//    }
//}

//void PResultNode::dump_buffer_to_file() {
//    for(int i = 0; i < buffer.size(); ++i) {
//        for(int j = 0; j < buffer[i].size(); ++j) {
//            switch(prototype->fieldTypes[j]) {
//            case VT_INT:
//                data_file << buffer[i][j].vint << " ";
//                break;
//            case VT_STRING:
//                data_file << buffer[i][j].vstr << " ";
//            }
//        }
//        data_file << std::endl;
//    }
//}

/* Physical nodes*/

PResultNode::PResultNode(PResultNode* left, PResultNode* right, LAbstractNode* p){
    this->left = left;
    this->right = right;
    this->prototype = p;

    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms =
            tp.tv_sec * 1000 + tp.tv_usec / 1000;
    std::stringstream filename;
    filename << "./tmp/op" << ms;
    data_filename = filename.str();
    data_file = std::ifstream(data_filename);

    buffer = std::vector(p->get_block_size());
    load_next_block();

    pos = 0;
    block = 0;
    block_pos = 0;

    // stats
    in_records = 0;
    if(left != 0) {
        in_records += left->get_out_records();
    }
    if(right != 0) {
        in_records += right->get_out_records();
    }
    out_records = 0;
}

PResultNode::~PResultNode(){
}

std::tuple<ErrCode, std::vector<Value>> PResultNode::GetRecord(){
    std::vector<Value> vals;

    if (pos == out_records) {
        return std::make_tuple(EC_FINISH, vals);
    }

    if (block_pos == prototype->get_block_size()) {
        load_next_block();
    }

    for(int i = 0; i < GetAttrNum(); i++){
        vals.push_back(data[pos][i]);
    }
    ++pos;
    return std::make_tuple(EC_OK, vals);
}
