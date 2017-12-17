#include<cstddef>
#include<algorithm>

#include "pcrossproductnode.h"

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p):
        PGetNextNode(left, right, p) {
    PGetNextNode* l = (PGetNextNode*)left;
    for(std::pair<bool, std::vector<std::vector<Value>>>
          l_data = l->GetNext();
          l_data.first;
          l_data = l->GetNext()) {
        for(auto row: l_data.second) {
            left_data.push_back(row);
        }
    }
    in_records += left_data.size();
}

PCrossProductNode::~PCrossProductNode() {
    delete left;
    delete right;
}

std::pair<bool, std::vector<std::vector<Value>>> PCrossProductNode::GetNext() {
    PGetNextNode* l = (PGetNextNode*)left;
    PGetNextNode* r = (PGetNextNode*)right;
    std::vector<std::vector<Value>> result;

    if(pos >= left_data.size()) {
        pos= 0;
        right_data = r->GetNext();
        if(!right_data.first) {
            mult = 0;
            return right_data;
        }
        in_records += mult * right_data.second.size();
    }
    for(auto r_row: right_data.second) {
        std::vector<Value> row(left_data[pos]);
        row.insert(row.end(), r_row.begin(), r_row.end());
        result.push_back(row);
    }
    out_records += mult * result.size();
    ++pos;
    return std::make_pair(
        (pos - 1) < left_data.size() || right_data.first, result);
}

void PCrossProductNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "NL-CROSSPRODUCT: ";
  for(auto names: prototype->fieldNames) {
    for(auto name: names) {
      std::cout << name << " ";
    }
  }
  std::cout << std::endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}
