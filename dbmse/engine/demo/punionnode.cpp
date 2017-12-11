#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include <string.h>
#include <vector>
#include <sstream>

#include "punionnode.h"
#include "../interface/basics.h"

std::pair<bool, std::vector<std::vector<Value>>> PUnionNode::GetNext() {
    PGetNextNode* l = (PGetNextNode*)left;
    std::pair<bool, std::vector<std::vector<Value>>> l_data = l->GetNext();
    if (l_data.first) {
        return l_data;
    }

    PGetNextNode* r = (PGetNextNode*)right;
    std::pair<bool, std::vector<std::vector<Value>>> r_data = r->GetNext();
    return r_data; // no matter what the bool flag shows
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
