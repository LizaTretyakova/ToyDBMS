#ifndef PPROJECTNODE_H
#define PPROJECTNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PProjectNode : public PGetNextNode{
public:
    PProjectNode(PGetNextNode* left, LAbstractNode* p);
    ~PProjectNode();
    virtual void Initialize() {}
    virtual std::pair<bool, std::vector<std::vector<Value>>> GetNext();
    virtual void Print(int indent);
private:
    LAbstractNode* child_proto;
};

#endif // PPROJECTNODE_H
