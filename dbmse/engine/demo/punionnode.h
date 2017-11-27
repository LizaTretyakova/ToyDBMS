#ifndef PUNIONNODE_H
#define PUNIONNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PUnionNode : public PGetNextNode{
  public:
    PUnionNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p):
        PGetNextNode(left, right, p) {}
    ~PUnionNode();
    virtual void Initialize();
    // print node
    virtual void Print(int indent);
};

#endif // PUNIONNODE_H
