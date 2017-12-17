#ifndef PJOINMERGESORTNODE_H
#define PJOINMERGESORTNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"
#include "pjoinnode.h"

class PJoinMergeSortNode : public PJoinNode {
  public:
    PJoinMergeSortNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p):
        PJoinNode(left, right, p) {
        std::sort(left_data.begin(), left_data.end());
        pos = left_data.size();
    }
    ~PJoinMergeSortNode();
    virtual std::vector<std::vector<Value>> join_blocks();
    virtual void Print(int indent);
};

#endif // PJOINMERGESORTNODE_H
