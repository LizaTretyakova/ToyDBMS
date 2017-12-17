// Toy DBMS, v0.4
// George Chernishev (c) 2016-2017, chernishev<at>google mail
// A task framework for undergraduate students at Saint-Petersburg Academic University, DBMS development course
// More details regarding the course can be found here: www.math.spbu.ru/user/chernishev/
// CHANGELOG:
// 0.4: no chance for efficiency competition, so, this year I reoriented task towards ideas:
//      1) tried to remove C code, now we are using lots of std:: stuff
//      2) implemented support for multiple attributes in the DBMS
//      3) code clean-up and restructurization
// 0.3: added:
//      1) support for restricting physical join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include<cstddef>
#include<algorithm>

#include "pjoinnode.h"

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right,
                     LAbstractNode* p): PGetNextNode(left, right, p){
  l = (PGetNextNode*)left;
  r = (PGetNextNode*)right;
  lp = l->prototype;
  rp = r->prototype;
  ln = lp->fieldNames;
  rn = rp->fieldNames;

  lpos = lp->find_pos(((LJoinNode*)prototype)->attr1, ((LJoinNode*)prototype)->attr2);
  rpos = rp->find_pos(((LJoinNode*)prototype)->attr1, ((LJoinNode*)prototype)->attr2);

  vt = lp->fieldTypes[lpos];

  for(std::pair<bool, std::vector<std::vector<Value>>>
        l_data = l->GetNext();
        l_data.first;
        l_data = l->GetNext()) {
      for(auto row: l_data.second) {
          left_data.push_back(row);
      }
  }

  pos = 0;
}

PJoinNode::~PJoinNode(){
  delete left;
  delete right;
}

std::vector<std::vector<Value>> PJoinNode::join_blocks() {
    std::vector<std::vector<Value>> result;
 //   for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < right_data.size(); j++){
            bool join = vt == VT_INT ?
                    ((int)left_data[pos][lpos] == (int)right_data[j][rpos]) :
                    ((std::string)left_data[pos][lpos] == (std::string)right_data[j][rpos]);

            if (!join) {
                continue;
            }

            std::vector<Value> tmp;
            for (int k = 0; k < ln.size(); k++){
                if(k != lpos){
                    tmp.push_back(left_data[pos][k]);
                }
            }
            for (int k = 0; k < rn.size(); k++){
                if(k != rpos){
                    tmp.push_back(right_data[j][k]);
                }
            }
            tmp.push_back(left_data[pos][lpos]);
            result.push_back(tmp);
        }
//    }
    ++pos;
    return result;
}

std::pair<bool, std::vector<std::vector<Value>>> PJoinNode::GetNext() {
    std::vector<std::vector<Value>> result;

    if(pos >= left_data.size()) {
        pos = 0;
        std::pair<bool, std::vector<std::vector<Value>>>
            rnxt = r->GetNext();
        right_data = rnxt.second;
        in_records += mult * right_data.size();
        if(!rnxt.first) {
            mult = 0;
            return rnxt;
        }
    }
    result = join_blocks();
    out_records += mult * result.size();
    return std::make_pair(true, result);
}

void PJoinNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "NL-JOIN: " << ((LJoinNode*)prototype)->attr1 <<"=" << ((LJoinNode*)prototype)->attr2 << std::endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}
