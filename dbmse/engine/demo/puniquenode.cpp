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
    int req = block_size;
    int res_size = std::min(req, block_size);

    // if there's something left in the storage, take it
    result.insert(result.end(), data.begin() + block_pos, std::min(data.end(), data.begin() + block_pos + res_size));
    block_pos = std::min((int)data.size(), block_pos + res_size);

    // while we are not full and still have entries
    while(result.size() < res_size && !finished) {
        std::pair<bool, std::vector<std::vector<Value>>> l_data = l->GetNext(); // (block_size);
        in_records += mult * l_data.second.size();
        for(int i; i < l_data.second.size(); ++i) {
            bool duplicate = false;
            for(int j = 0; j < data.size(); ++j) {
                if(equals(l_data.second[i], data[j])) {
                    duplicate = true;
                    break;
                }
            }
            if(!duplicate) {
                if(result.size() < res_size) {
                    result.push_back(l_data.second[i]);
                    ++block_pos;
                }
                data.push_back(l_data.second[i]);
            }
        }
        finished = !l_data.first;
    }

    out_records += mult * result.size();
    if(finished) {
        mult = 0;
    }
    return std::make_pair(!finished || result.size() > 0, result);
}

void PUniqueNode::Print(int indent){
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << "UNIQUE " << std::endl;
    left->Print(indent + 2);
}

