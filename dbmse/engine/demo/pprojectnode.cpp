#include<cstddef>
#include<algorithm>

#include "pprojectnode.h"

PProjectNode::PProjectNode(PGetNextNode* child, LAbstractNode* p): PGetNextNode(child, NULL, p){
  Initialize();
}

PProjectNode::~PProjectNode() {
  delete left;
}

void PProjectNode::Initialize() {
    PGetNextNode* child = (PGetNextNode*)left;
    std::vector<std::vector<Value>> child_data = child->GetNext();
    LAbstractNode* child_proto = child->prototype;

    for(auto row: child_data) {
        std::vector<Value> tmp;
        for(int i = 0; i < child_proto->fieldNames.size(); ++i) {
            for(auto name: child_proto->fieldNames[i]) {
                if(prototype->contains_str(name)) {
                    tmp.push_back(row[i]);
                    break;
                }
            }
        }
        data.push_back(tmp);
    }
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
