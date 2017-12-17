#include <cstddef>
#include <algorithm>
#include <unordered_map>

#include "pjoinhashnode.h"
#include "pjoinnode.h"

PJoinHashNode::~PJoinHashNode() {
    delete left;
    delete right;
}

std::vector<std::vector<Value>> PJoinHashNode::join_blocks() {
    std::vector<std::vector<Value>> result;
    if(vt == VT_INT) {
        for(int i = 0; i < right_data.size(); ++i) {
            if(int_table.count(right_data[i][rpos]) > 0) {
                for(int j: int_table[right_data[i][rpos]]) {
                    std::vector<Value> tmp;
                    for (int k = 0; k < ln.size(); k++){
                        tmp.push_back(left_data[j][k]);
                    }
                    for (int k = 0; k < rn.size(); k++){
                        if(k != rpos){
                            tmp.push_back(right_data[i][k]);
                        }
                    }
                    result.push_back(tmp);
                }
            }
        }
    } else {
        for(int i = 0; i < right_data.size(); ++i) {
            if(string_table.count(right_data[i][rpos]) > 0) {
                for(int j: string_table[right_data[i][rpos]]) {
                    std::vector<Value> tmp;
                    for (int k = 0; k < ln.size(); k++){
                        tmp.push_back(left_data[j][k]);
                    }
                    for (int k = 0; k < rn.size(); k++){
                        if(k != rpos){
                            tmp.push_back(right_data[i][k]);
                        }
                    }
                    result.push_back(tmp);
                }
            }
        }
    }
    ++pos;
    return result;
}

void PJoinHashNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "NL-HASH-JOIN: " << ((LJoinNode*)prototype)->attr1 <<"=" << ((LJoinNode*)prototype)->attr2 << std::endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}
