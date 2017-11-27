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

#include "pgetnextnode.h"

PGetNextNode::PGetNextNode():
    PResultNode(NULL, NULL, NULL), block_pos(0) {
  Initialize();
}

PGetNextNode::PGetNextNode(
        PResultNode* left,
        PResultNode* right,
        LAbstractNode* source):
    PResultNode(left, right, source), block_pos(0) {
  Initialize();
}

void PGetNextNode::Initialize(){
  return;
}

// returns (true, ...) if not ended,
// (false, ...) otherwise
std::pair<bool, std::vector<std::vector<Value>>> PGetNextNode::GetNext() {
    int block_size = prototype->get_block_size();
    block_pos = block_pos % data.size();

    std::vector<std::vector<Value> >
        result(std::min(block_size, (int)data.size() - block_pos));
    for(; block_pos < data.size(); ++block_pos) {
        result.push_back(data[block_pos]);
    }

    return std::make_pair(block_pos == data.size(), result);
}

int PGetNextNode::GetAttrNum(){
  return prototype->fieldNames.size();
}


