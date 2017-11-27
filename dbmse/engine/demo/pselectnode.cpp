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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include <string.h>
#include <vector>
#include <sstream>

#include "pselectnode.h"

PSelectNode::PSelectNode(){}

PSelectNode::PSelectNode(LAbstractNode* p): PGetNextNode() {
    this->table = ((LSelectNode*)p)->GetBaseTable();

    std::tuple<int, Predicate> tmp_p = ((LSelectNode*)p)->GetNextPredicate();
    while(std::get<0>(tmp_p) != 1) {
        this->predicate.push_back(std::get<1>(tmp_p));
        tmp_p = ((LSelectNode*)p)->GetNextPredicate();
    }

    this->prototype = p;
    pos = 0;
    data.clear();
    Initialize();
}

PSelectNode::PSelectNode(LAbstractNode* p, std::vector<Predicate> predicate): PGetNextNode(){
  this->table = ((LSelectNode*)p)->GetBaseTable();
  this->predicate = predicate;
  this->prototype = p;
  pos = 0;
  data.clear();
  Initialize();
}

PSelectNode::~PSelectNode(){
}

void PSelectNode::Initialize(){
  std::string line;
  std::string word;
  std::ifstream f(table.relpath);
  if(f.is_open()){
    // skipping first 4 lines
    getline(f, line);
    getline(f, line);
    getline(f, line);
    getline(f, line);

    while(getline(f, line)){
      std::vector<Value> tmp;
      std::istringstream iss(line, std::istringstream::in);
      int i = 0;
      bool passes_pred = true;

      while (passes_pred && iss >> word){
        Value h;
        if (prototype->fieldTypes[i] == VT_INT) {
          h = Value(std::stoi(word));
        } else {
          h = Value(word);
        }

        for(int j = 0; j < predicate.size() && passes_pred; ++j) {
            if(predicate[j].attribute == i) {
                passes_pred &= predicate[j].apply(h);
            }
        }

        tmp.push_back(h);
        i++;
      }
      if(passes_pred) {
        data.push_back(tmp);
      }
    }
    f.close();
  } else std::cout << "Unable to open file";
}

std::vector<std::vector<Value>> PSelectNode::GetNext(){
  return data;
}

void PSelectNode::Print(int indent){
    for (int i = 0; i < indent; i++){
        std::cout << " ";
    }
    std::cout<<"SCAN "<<table.relpath<<" with predicate ";
    if(predicate.size() != 0) {// TODO: output only the first pred instead of all
        for (int i = 0; i < predicate.size(); ++i) {
            if (i > 0) {
                std::cout << " and ";
            }
            std::cout << predicate[i];
        }
    } else {
        std::cout << "NULL";
    }
    std::cout << std::endl;
}

