#include <cstddef>
#include <algorithm>
#include <unordered_map>

#include "pjoinhashnode.h"
#include "pjoinnode.h"

PJoinHashNode::~PJoinHashNode() {
    delete left;
    delete right;
}

void PJoinHashNode::join_blocks(
        std::vector<std::vector<Value>> lres,
        std::vector<std::vector<Value>> rres,
        std::vector<std::vector<std::string>> ln,
        std::vector<std::vector<std::string>> rn,
        std::ptrdiff_t lpos,
        std::ptrdiff_t rpos,
        ValueType vt) {
    pos = 0;
    data.clear();
    if(vt == VT_INT) {
        join_blocks_int(lres, rres, ln, rn, lpos, rpos);
    } else {
        join_blocks_str(lres, rres, ln, rn, lpos, rpos);
    }
}

void PJoinHashNode::join_blocks_int(
        std::vector<std::vector<Value>> lres,
        std::vector<std::vector<Value>> rres,
        std::vector<std::vector<std::string>> ln,
        std::vector<std::vector<std::string>> rn,
        std::ptrdiff_t lpos,
        std::ptrdiff_t rpos) {
    std::unordered_map<int, std::vector<int>> table;

    for(int i = 0; i < lres.size(); ++i) {
        table[lres[i][lpos]].push_back(i);
    }

    for(int i = 0; i < rres.size(); ++i) {
        if(table.count(rres[i][rpos]) > 0) {
            for(int j: table[rres[i][rpos]]) {
                std::vector<Value> tmp;
                for (int k = 0; k < ln.size(); k++){
                    tmp.push_back(lres[j][k]);
                }
                for (int k = 0; k < rn.size(); k++){
                    if(k != rpos){
                        tmp.push_back(rres[i][k]);
                    }
                }
                data.push_back(tmp);
            }
        }
    }
}

void PJoinHashNode::join_blocks_str(
        std::vector<std::vector<Value>> lres,
        std::vector<std::vector<Value>> rres,
        std::vector<std::vector<std::string>> ln,
        std::vector<std::vector<std::string>> rn,
        std::ptrdiff_t lpos,
        std::ptrdiff_t rpos) {
    std::unordered_map<std::string, std::vector<int>> table;

    for(int i = 0; i < lres.size(); ++i) {
        table[lres[i][lpos]].push_back(i);
    }

    for(int i = 0; i < rres.size(); ++i) {
        if(table.count(rres[i][rpos]) > 0) {
            for(int j: table[rres[i][rpos]]) {
                std::vector<Value> tmp;
                for (int k = 0; k < ln.size(); k++){
                    tmp.push_back(lres[j][k]);
                }
                for (int k = 0; k < rn.size(); k++){
                    if(k != rpos){
                        tmp.push_back(rres[i][k]);
                    }
                }
                data.push_back(tmp);
            }
        }
    }
}

void PJoinHashNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "NL-HASH-JOIN: " << ((LJoinNode*)prototype)->attr1 <<"=" << ((LJoinNode*)prototype)->attr2 << std::endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}
