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

#ifndef PJOINNODE_H
#define PJOINNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PJoinNode : public PGetNextNode {
public:
    PJoinNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p);
    ~PJoinNode();
    virtual void Initialize() {}
    virtual std::pair<bool, std::vector<std::vector<Value>>> PUnionNode::GetNext();
    virtual void Print(int indent);
protected:
    virtual void join_blocks(
        std::vector<std::vector<Value>> lres,
        std::vector<std::vector<Value>> rres,
        std::vector<std::vector<std::string>> ln,
        std::vector<std::vector<std::string>> rn,
        std::ptrdiff_t lpos,
        std::ptrdiff_t rpos,
        ValueType vt);
  private:
    int pos;
};

#endif // PJOINNODE_H
