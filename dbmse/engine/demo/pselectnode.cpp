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
#include <ostream>
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

    std::string line;
    std::ifstream f(table.relpath);
    if(f.is_open()){
        // skipping first 4 lines
        getline(f, line);
        getline(f, line);
        getline(f, line);
        getline(f, line);
        file_start = file_pos = f.tellg();
    } else {
      std::cout << "Unable to open file";
    }
}

PSelectNode::PSelectNode(LAbstractNode* p, std::vector<Predicate> predicate): PGetNextNode() {
    this->table = ((LSelectNode*)p)->GetBaseTable();
    this->predicate = predicate;
    this->prototype = p;

    std::string line;
    std::ifstream f(table.relpath);
    if(f.is_open()){
        // skipping first 4 lines
        getline(f, line);
        getline(f, line);
        getline(f, line);
        getline(f, line);
        file_start = file_pos = f.tellg();
    } else {
      std::cout << "Unable to open file";
    }
}

PSelectNode::~PSelectNode(){
}

std::pair<bool, std::vector<std::vector<Value>>> PSelectNode::GetNext() {
    int block_size = prototype->get_block_size();
    std::string line;
    std::string word;
    std::vector<std::vector<Value>> result;
    int cnt = 0;
    std::ifstream f(table.relpath);
    f.seekg(file_pos);
    if(f.is_open()){
        for(; cnt < block_size && getline(f, line); ++cnt) {
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
                result.push_back(tmp);
            }
        }
    } else {
        std::cout << "Unable to open file";
    }
    if(cnt == 0) {
        file_pos = file_start;
        mult = 0;
        return std::make_pair(false, result);
    } else {
        file_pos = f.tellg();
    }
    in_records += mult * result.size();
    out_records += mult * result.size();
    return std::make_pair(true, result);
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

