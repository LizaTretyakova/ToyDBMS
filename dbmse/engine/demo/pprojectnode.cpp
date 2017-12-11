#include<cstddef>
#include<algorithm>

#include "pprojectnode.h"

PProjectNode::PProjectNode(PGetNextNode* child, LAbstractNode* p): PGetNextNode(child, NULL, p){
}

PProjectNode::~PProjectNode() {
  delete left;
}

std::pair<bool, std::vector<std::vector<Value>>> PProjectNode::GetNext() {
    PGetNextNode* child = (PGetNextNode*)left;
    std::pair<bool, std::vector<std::vector<Value>>>
        child_data = child->GetNext();
    LAbstractNode* child_proto = child->prototype;
    std::vector<std::vector<Value>> result;

    for(auto row: child_data.second) {
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
    return std::make_pair(child_data.first, result);
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
