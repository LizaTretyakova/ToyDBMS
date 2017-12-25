#include<cstddef>
#include<algorithm>

#include "pcrossproductnode.h"

void PCrossProductNode::left_load() {
    PGetNextNode* l = (PGetNextNode*)left;
    std::pair<bool, std::vector<std::vector<Value>>> nxt = l->GetNext();
    left_data = nxt.second;
    in_records += left_data.size();
    if(!nxt.first) {
        left_state = FIN;
        left_mult = 0;
    } else {
        left_state = PROGR;
    }
}

void PCrossProductNode::right_load() {
    PGetNextNode* r = (PGetNextNode*)right;
    std::pair<bool, std::vector<std::vector<Value>>> nxt = r->GetNext();
    right_data = nxt.second;
    in_records += right_data.size();
    if(!nxt.first) {
        right_state = FIN;
        right_mult = 0;
        mult = 0;
    } else {
        right_state = PROGR;
    }
}

void PCrossProductNode::do_match() {
    for(int i = 0; i < right_data.size(); ++i) {
        std::vector<Value> tmp;
        tmp.insert(tmp.end(),
            left_data[left_pos].begin(), left_data[left_pos].end());
        tmp.insert(tmp.end(),
            right_data[i].begin(), right_data[i].end());
        data.push_back(tmp);
        std::cout << "***data.push_back left_pos " << left_pos
                  << " right i " << i << std::endl;
    }
}

void PCrossProductNode::advance_left() {
    ++left_pos;
    if(left_pos == left_data.size()) {
        left_state = NEXT_BLOCK;
        left_pos = 0;
    }
}

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p):
        PGetNextNode(left, right, p),
        left_pos(0),
        left_mult(1),
        right_mult(1),
        left_state(NONE),
        right_state(NONE) {
//    PGetNextNode* l = (PGetNextNode*)left;
//    for(std::pair<bool, std::vector<std::vector<Value>>>
//          l_data = l->GetNext();
//          l_data.first;
//          l_data = l->GetNext()) {
//        for(auto row: l_data.second) {
//            left_data.push_back(row);
//        }
//    }
//    in_records += left_data.size();
}

PCrossProductNode::~PCrossProductNode() {
    delete left;
    delete right;
}

std::pair<bool, std::vector<std::vector<Value>>> PCrossProductNode::GetNext() {
//    PGetNextNode* l = (PGetNextNode*)left;
//    PGetNextNode* r = (PGetNextNode*)right;
//    std::vector<std::vector<Value>> result;

//    if(pos >= left_data.size()) {
//        pos= 0;
//        right_data = r->GetNext();
//        if(!right_data.first) {
//            mult = 0;
//            return right_data;
//        }
//        in_records += mult * right_data.second.size();
//    }
//    for(auto r_row: right_data.second) {
//        std::vector<Value> row(left_data[pos]);
//        row.insert(row.end(), r_row.begin(), r_row.end());
//        result.push_back(row);
//    }
//    out_records += mult * result.size();
//    ++pos;
//    return std::make_pair(
//        (pos - 1) < left_data.size() || right_data.first, result);

    // finished <=> right_state == FIN
    int req = block_size;
    while(data.size() < std::min(req, block_size) && !finished) {
        std::cout << "data size: " << data.size() << std::endl;
        std::cout << "left_state: " << left_state << std::endl;
        std::cout << "right_state: " << right_state << std::endl;
        switch(left_state) {
        case NONE:
            left_load();
            right_load();
            do_match();
            advance_left();
            break;
        case PROGR:
            do_match();
            advance_left();
            break;
        case NEXT_BLOCK:
            left_load();
            break;
        case FIN:
            right_load();
            // the data is over
            if(right_state == FIN) {
                finish();
                std::cout << "***OK1" << std::endl;
                return form_result(req);
            }
            left->rewind();
            left_pos = 0;
            left_load();
            break;
        }
    }
    std::cout << "***OK2" << std::endl;
    return form_result(req);
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
