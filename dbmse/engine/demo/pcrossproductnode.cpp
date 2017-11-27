#include<cstddef>
#include<algorithm>

#include "pcrossproductnode.h"

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p):
    PGetNextNode(left, right, p) {
    Initialize();
}

PCrossProductNode::~PCrossProductNode() {
    delete left;
    delete right;
}

std::vector<std::vector<Value>> PCrossProductNode::GetNext() {
    int block_size = prototype->get_block_size();
    std::vector<std::vector<Value> >
        result(min(block_size, pos % block_size));
    for(int i = block * block_size; i < pos; ++i) {
        result.push_back(data[i]);
    }
    return result;
}

void PCrossProductNode::Initialize() {
    PGetNextNode* l = (PGetNextNode*)left;
    PGetNextNode* r = (PGetNextNode*)right;
    std::vector<std::vector<Value>> l_data = l->GetNext();
    std::vector<std::vector<Value>> r_data = r->GetNext();

    for(auto l_row: l_data) {
        for(auto r_row: r_data) {
            std::vector<Value> row(l_row);
            row.insert(row.end(), r_row.begin(), r_row.end());
            data.push_back(row);
        }
    }
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
