#ifndef PUNIQUENODE_H
#define PUNIQUENODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PUniqueNode : public PGetNextNode{
  public:
    PUniqueNode(PGetNextNode* child, LAbstractNode* p);
    ~PUniqueNode();
    virtual std::vector<std::vector<Value>> GetNext();
    virtual void Initialize();
    // print node
    virtual void Print(int indent);
  private:
    bool equals(std::vector<Value> a1, std::vector<Value> a2);
};

#endif // PUNIQUENODE_H
