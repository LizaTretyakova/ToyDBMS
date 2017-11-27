#ifndef PUNIONNODE_H
#define PUNIONNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PUnionNode : public PGetNextNode{
public:
    PUnionNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p):
        PGetNextNode(left, right, p), left_records(0), right_records(0) {}
    ~PUnionNode();
    virtual void Initialize();
    // print node
    virtual void Print(int indent);

protected:
    int left_records;
    int right_records;
};

#endif // PUNIONNODE_H
