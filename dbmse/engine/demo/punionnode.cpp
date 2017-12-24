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
//    if(doing_first) {
//		PGetNextNode* l = (PGetNextNode*)left;
//		std::pair<bool, std::vector<std::vector<Value>>> l_data = l->GetNext();
//		if (l_data.first) {
//		    in_records += mult * l_data.second.size();
//		    out_records += mult * l_data.second.size();
//		    doing_first = false;
//		    return l_data;
//		}
//    }
//    PGetNextNode* r = (PGetNextNode*)right;
//    std::pair<bool, std::vector<std::vector<Value>>> r_data = r->GetNext();
//    in_records += mult * r_data.second.size();
//    out_records += mult * r_data.second.size();
//    if(!r_data.first) {
//	mult = 0;
//	doing_first = true;
//    }
//    return r_data; // no matter what the bool flag shows
    int req = block_size;
    while(data.size() < block_size && !finished) {
        if(doing_first) {
            PGetNextNode* l = (PGetNextNode*)left;
            std::pair<bool, std::vector<std::vector<Value>>> l_data = l->GetNext(); // (block_size);
            if (l_data.first) {
                in_records += mult * l_data.second.size();
                out_records += mult * l_data.second.size();
                // fill the buffer
                data.insert(data.end(), l_data.second.begin(), l_data.second.end());
            } else {
                doing_first = false;
            }
        } else {
            PGetNextNode* r = (PGetNextNode*)right;
            std::pair<bool, std::vector<std::vector<Value>>> r_data = r->GetNext(); // (block_size);
            if (r_data.first) {
                in_records += mult * r_data.second.size();
                out_records += mult * r_data.second.size();
                // fill the buffer
                data.insert(data.end(), r_data.second.begin(), r_data.second.end());
            } else {
                finish();
                doing_first = true;
                mult = 0;
            }
        }
    }
    int res_size = std::min(req, (int)data.size());
    std::vector<std::vector<Value>> result;
    result.insert(result.end(), data.end() - res_size, data.end());
    data.resize(data.size() - res_size);
    return make_pair(!finished || result.size() > 0, result);
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
