#ifndef PCROSSPRODUCTNODE_H
#define PCROSSPRODUCTNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PCrossProductNode : public PGetNextNode{
  public:
    PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p);
    ~PCrossProductNode();
    virtual void Initialize() {}
    virtual std::pair<bool, std::vector<std::vector<Value>>> GetNext();
    virtual void Print(int indent);

protected:
    int pos;
    std::vector<std::vector<Value>> left_data;
    std::pair<bool, std::vector<std::vector<Value>>> right_data;
};

#endif // PCROSSPRODUCTNODE_H
