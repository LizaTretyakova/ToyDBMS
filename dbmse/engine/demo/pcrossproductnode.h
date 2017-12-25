#ifndef PCROSSPRODUCTNODE_H
#define PCROSSPRODUCTNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

enum State {
    NONE = -1,
    PROGR = 0,
    NEXT_BLOCK = 1,
    FIN = 2
};

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
//    std::pair<bool, std::vector<std::vector<Value>>> right_data;
    std::vector<std::vector<Value>> right_data;
    int left_pos;
    int left_mult;
    int right_mult;
    State left_state;
    State right_state;

    void left_load();
    void right_load();
    void do_match();
    void advance_left();
};

#endif // PCROSSPRODUCTNODE_H
