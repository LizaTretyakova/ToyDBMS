#ifndef PJOINMERGESORTNODE_H
#define PJOINMERGESORTNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"
#include "pjoinnode.h"

class PJoinMergeSortNode : public PJoinNode {
  public:
    PJoinMergeSortNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p):
        PJoinNode(left, right, p) {}
    ~PJoinMergeSortNode();
    virtual void join_blocks(
            std::vector<std::vector<Value>> lres,
            std::vector<std::vector<Value>> rres,
            std::vector<std::vector<std::string>> ln,
            std::vector<std::vector<std::string>> rn,
            std::ptrdiff_t lpos,
            std::ptrdiff_t rpos,
            ValueType vt);
    virtual void Print(int indent);
};

#endif // PJOINMERGESORTNODE_H
