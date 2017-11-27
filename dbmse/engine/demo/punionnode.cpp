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
        for(std::vector<Value> val: l_data.second) {
            data.push_back(val);
        }
    }
    for(std::pair<bool, std::vector<std::vector<Value>>> l_data = l->GetNext();
            r_data.first; r_data = r->GetNext()) {
        for(std::vector<Value> val: r_data.second) {
            data.push_back(val);
        }
    }
}

void Print(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "UNION " << std::endl;
    left->Print(indent + 2);
}
