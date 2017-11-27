#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include <string.h>
#include <vector>
#include <sstream>

#include "punionnode.h"
#include "../interface/basics.h"

void PUnionNode::Initialize() {
    PGetNextNode* l = (PGetNextNode*)left;
    PGetNextNode* r = (PGetNextNode*)right;

    for(std::pair<bool, std::vector<std::vector<Value>>> l_data = l->GetNext();
            l_data.first; l_data = l->GetNext()) {
        left_records += l_data.second.size();

        for(std::vector<Value> val: l_data.second) {
            data.push_back(val);
        }
    }
    for(std::pair<bool, std::vector<std::vector<Value>>> r_data = r->GetNext();
            r_data.first; r_data = r->GetNext()) {
        right_records += r_data.second.size();

        for(std::vector<Value> val: r_data.second) {
            data.push_back(val);
        }
    }
}

PUnionNode::~PUnionNode(){
    delete left;
    delete right;
}

void PUnionNode::Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "UNION " << std::endl;
    left->Print(indent + 2);
}
