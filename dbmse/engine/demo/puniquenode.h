#ifndef PUNIQUENODE_H
#define PUNIQUENODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PUniqueNode : public PGetNextNode{
  public:
    PUniqueNode(PGetNextNode* child, LAbstractNode* p);
    ~PUniqueNode();
    virtual void Initialize() {}
    virtual std::pair<bool, std::vector<std::vector<Value>>> GetNext();
    // print node
    virtual void Print(int indent);
  private:
    bool equals(std::vector<Value> a1, std::vector<Value> a2);
};

#endif // PUNIQUENODE_H
