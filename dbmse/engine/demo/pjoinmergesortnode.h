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
};

#endif // PJOINMERGESORTNODE_H
