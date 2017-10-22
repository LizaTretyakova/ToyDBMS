#ifndef PPROJECTNODE_H
#define PPROJECTNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PProjectNode : public PGetNextNode{
  public:
    PProjectNode(PGetNextNode* left, LAbstractNode* p);
    ~PProjectNode();
    virtual std::vector<std::vector<Value>> GetNext();
    virtual void Initialize();
    virtual void Print(int indent);
};

#endif // PPROJECTNODE_H
