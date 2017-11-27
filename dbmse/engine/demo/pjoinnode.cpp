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

#include<cstddef>
#include<algorithm>

#include "pjoinnode.h"

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right,
                     LAbstractNode* p): PGetNextNode(left, right, p){
  pos = 0;
  Initialize();
}

PJoinNode::~PJoinNode(){
  delete left;
  delete right;
}

void PJoinNode::join_blocks(
        std::vector<std::vector<Value>> lres,
        std::vector<std::vector<Value>> rres,
        std::vector<std::vector<std::string>> ln,
        std::vector<std::vector<std::string>> rn,
        std::ptrdiff_t lpos,
        std::ptrdiff_t rpos,
        ValueType vt) {
    for (int i = 0; i < lres.size(); i++) {
        for (int j = 0; j < rres.size(); j++){
            bool join = vt == VT_INT ?
                    ((int)lres[i][lpos] == (int)rres[j][rpos]) :
                    ((std::string)lres[i][lpos] == (std::string)rres[j][rpos]);

            if (!join) {
                continue;
            }

            std::vector<Value> tmp;
            for (int k = 0; k < ln.size(); k++){
                if(k != lpos){
                    tmp.push_back(lres[i][k]);
                }
            }
            for (int k = 0; k < rn.size(); k++){
                if(k != rpos){
                    tmp.push_back(rres[j][k]);
                }
            }
            tmp.push_back(lres[i][lpos]);
            data.push_back(tmp);
        }
    }
}

void PJoinNode::Initialize(){
    PGetNextNode* l = (PGetNextNode*)left;
    PGetNextNode* r = (PGetNextNode*)right;
    LAbstractNode* lp = l->prototype;
    LAbstractNode* rp = r->prototype;
    std::vector<std::vector<std::string>> ln = lp->fieldNames;
    std::vector<std::vector<std::string>> rn = rp->fieldNames;

    LAbstractNode* p = prototype;
    std::ptrdiff_t lpos = lp->find_pos(((LJoinNode*)prototype)->attr1, ((LJoinNode*)prototype)->attr2);
    std::ptrdiff_t rpos = rp->find_pos(((LJoinNode*)prototype)->attr1, ((LJoinNode*)prototype)->attr2);

    ValueType vt = lp->fieldTypes[lpos];

    std::pair<bool, std::vector<std::vector<Value>>> lres = l->GetNext();
    std::pair<bool, std::vector<std::vector<Value>>> rres = r->GetNext();
    while(lres.first) {
        while(rres.first) {
            join_blocks(
                lres.second, rres.second,
                ln, rn, lpos, rpos, vt);
            rres = r->GetNext();
        }
        lres = l->GetNext();
    }
}

void PJoinNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "NL-JOIN: " << ((LJoinNode*)prototype)->attr1 <<"=" << ((LJoinNode*)prototype)->attr2 << std::endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}
