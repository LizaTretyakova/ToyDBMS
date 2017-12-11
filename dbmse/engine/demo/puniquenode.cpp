#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include <string.h>
#include <vector>
#include <sstream>

#include "puniquenode.h"
#include "../interface/basics.h"

PUniqueNode::PUniqueNode(PGetNextNode* child, LAbstractNode* p):
    PGetNextNode(child, NULL, p) {
}

PUniqueNode::~PUniqueNode(){
    delete left;
}

bool PUniqueNode::equals(std::vector<Value> a1, std::vector<Value> a2) {
    if(a1.size() != a2.size()) {
        return false;
    }
    for(int i = 0; i < a1.size(); ++i) {
        if(a1[i].vtype != a2[i].vtype) {
            return false;
        }
        switch(a1[i].vtype) {
        case VT_INT:
            if(a1[i].vint != a2[i].vint) {
                return false;
            }
            break;
        case VT_STRING:
            if(a1[i].vstr != a2[i].vstr) {
                return false;
            }
            break;
        }
    }
    return true;
}

std::pair<bool, std::vector<std::vector<Value>>> PUniqueNode::GetNext() {
    PGetNextNode* l = (PGetNextNode*)left;
    std::vector<std::vector<Value>> result;
    std::pair<bool, std::vector<std::vector<Value>>> l_data = l->GetNext();
    int block_size = prototype->get_block_size();

    for(int i = 0; i < block_size && i < l_data.second.size(); ++i) {
        bool duplicate = false;
        for(int j = i + 1; j < l_data.second.size(); ++j) {
            if(equals(l_data.second[i], l_data.second[j])) {
                duplicate = true;
                break;
            }
        }
        std::cerr << std::endl;
        if(!duplicate) {
            std::vector<Value> row(l_data.second[i]);
            result.push_back(row);
        }
    }

    return std::make_pair(l_data.first, result);
}

void PUniqueNode::Print(int indent){
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "UNIQUE " << std::endl;
    left->Print(indent + 2);
}

