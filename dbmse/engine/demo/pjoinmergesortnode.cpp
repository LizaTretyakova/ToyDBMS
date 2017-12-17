#include <algorithm>
#include <cstddef>
#include <cstring>
#include <unordered_map>

#include "pjoinmergesortnode.h"
#include "pjoinnode.h"

PJoinMergeSortNode::~PJoinMergeSortNode() {
    delete left;
    delete right;
}

std::vector<std::vector<Value> > PJoinMergeSortNode::join_blocks() {
    int lit = 0; // left iterator
    int rit = 0; // right iterator
    std::vector<std::vector<Value>> result;
    pos = left_data.size();
    std::sort(right_data.begin(), right_data.end());

    while(lit < left_data.size() && rit < right_data.size()) {
        bool join = vt == VT_INT ?
                ((int)left_data[lit][lpos] == (int)right_data[rit][rpos]) :
                ((std::string)left_data[lit][lpos] == (std::string)right_data[rit][rpos]);

        if(!join) {
            switch(vt) {
            case VT_INT:
                if((int)left_data[lit][lpos] < (int)right_data[rit][rpos]) {
                    ++lit;
                } else {
                    ++rit;
                }
                break;
            case VT_STRING:
                if(std::strcmp(((std::string)left_data[lit][lpos]).c_str(), ((std::string)right_data[rit][rpos]).c_str()) < 0) {
                    ++lit;
                } else {
                    ++rit;
                }
                break;
            }
            continue;
        }

        std::vector<Value> tmp;
        for (int k = 0; k < ln.size(); k++){
            tmp.push_back(left_data[lit][k]);
        }
        for (int k = 0; k < rn.size(); k++){
            if(k != rpos){
                tmp.push_back(right_data[rit][k]);
            }
        }
        result.push_back(tmp);
    }
    return result;
}

void PJoinMergeSortNode::Print(int indent){
    for (int i = 0; i < indent; i++){
    std::cout << " ";
    }
    std::cout << "NL-MS-JOIN: " << ((LJoinNode*)prototype)->attr1 <<"=" << ((LJoinNode*)prototype)->attr2 << std::endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}
