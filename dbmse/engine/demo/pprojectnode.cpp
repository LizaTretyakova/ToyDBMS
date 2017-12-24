#include<cstddef>
#include<algorithm>

#include "pprojectnode.h"

PProjectNode::PProjectNode(PGetNextNode* child, LAbstractNode* p): PGetNextNode(child, NULL, p){
    child_proto = child->prototype;
}

PProjectNode::~PProjectNode() {
  delete left;
}

std::pair<bool, std::vector<std::vector<Value>>> PProjectNode::GetNext() {
    int req = block_size;

    // Invariant: fill data 'till it's full
    // (important: 'full' == enough entries to give,
    // but we take from the underlying levels by chunks
    // of our block_size, no matter what's required)
    // and then give them further up, emptying data again.
    int res_size = std::min(req, block_size);
    while(data.size() < res_size && !finished) {
        PGetNextNode* l = (PGetNextNode*)left;
        std::pair<bool, std::vector<std::vector<Value>>> l_data = l->GetNext(); // (block_size);
        if (l_data.first) {
           in_records += mult * l_data.second.size();
           out_records += mult * l_data.second.size();
           // fill the buffer
           data.insert(data.end(), l_data.second.begin(), l_data.second.end());
        } else {
            finish(); // aka finished = true;
            mult = 0;
        }
    }

    std::vector<std::vector<Value>> result;
    int leftover = std::max(0, (int)data.size() - res_size);
    for(int it = leftover; it < data.size(); ++it) {
        std::vector<Value> row = data[it];
        std::vector<Value> tmp;
        for(int i = 0; i < child_proto->fieldNames.size(); ++i) {
            for(auto name: child_proto->fieldNames[i]) {
                if(prototype->contains_str(name)) {
                    tmp.push_back(row[i]);
                    break;
                }
            }
        }
        result.push_back(tmp);
    }
    out_records += mult * result.size();
    data.resize(leftover);
    return std::make_pair(result.size() != 0, result);
}

void PProjectNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "NL-PROJECT: ";
  for(auto names: prototype->fieldNames) {
    std::cout << "(";
    for(auto name: names) {
      std::cout << name << " ";
    }
    std::cout << ") ";
  }
  std::cout << std::endl;
  left->Print(indent + 2);
}
