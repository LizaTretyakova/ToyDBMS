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

void PCrossProductNode::Initialize() {
    PGetNextNode* l = (PGetNextNode*)left;
    PGetNextNode* r = (PGetNextNode*)right;

    std::pair<bool, std::vector<std::vector<Value>>>
        l_data = l->GetNext();
    while(l_data.first) {
        std::pair<bool, std::vector<std::vector<Value>>>
            r_data = r->GetNext();
        while(r_data.first) {
            in_records += l_data.second.size();
            in_records += r_data.second.size();

            for(auto l_row: l_data.second) {
                for(auto r_row: r_data.second) {
                    std::vector<Value> row(l_row);
                    row.insert(row.end(), r_row.begin(), r_row.end());
                    data.push_back(row);
                }
            }
            r_data = r->GetNext();
        }
        l_data = l->GetNext();
    }
    out_records = data.size();
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
