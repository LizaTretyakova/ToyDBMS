#ifndef PCROSSPRODUCTNODE_H
#define PCROSSPRODUCTNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PCrossProductNode : public PGetNextNode{
  public:
    PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p);
    ~PCrossProductNode();
    virtual std::vector<std::vector<Value>> GetNext();
    virtual void Initialize();
    virtual void Print(int indent);
};

#endif // PCROSSPRODUCTNODE_H
