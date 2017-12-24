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
    // because it's useless to unset `finished` and `data`
    // in case of this node
//    virtual void rewind();
  private:
    bool equals(std::vector<Value> a1, std::vector<Value> a2);
};

#endif // PUNIQUENODE_H
