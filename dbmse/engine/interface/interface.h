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

#ifndef INTERFACE_H
#define INTERFACE_H

#include <algorithm>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "basics.h"

/* Logical nodes (query) */

class LAbstractNode{
  public:
    LAbstractNode(LAbstractNode* left, LAbstractNode* right, int block_size);
    virtual ~LAbstractNode();
    LAbstractNode* GetLeft();
    LAbstractNode* GetRight();
    int get_block_size();

    // schema-related info
    std::vector<std::vector<std::string>> fieldNames;
    std::vector<ValueType> fieldTypes;
    std::vector<COLUMN_SORT> fieldOrders;

    // find the first occurence of either of the two strings
    std::ptrdiff_t find_pos(std::string s1, std::string s2) {
        for (int i = 0; i < fieldNames.size(); i++){
            std::ptrdiff_t pos1 = std::find(fieldNames[i].begin(), fieldNames[i].end(), s1) - fieldNames[i].begin();
            std::ptrdiff_t pos2 = std::find(fieldNames[i].begin(), fieldNames[i].end(), s2) - fieldNames[i].begin();

            if(pos1 <= fieldNames.size() || pos2 <= fieldNames.size()){
                return i;
            }
        }
    }

    bool contains_str(int attr, std::string s) {
        return std::find(fieldNames[attr].begin(), fieldNames[attr].end(), s)
                < fieldNames[attr].end();
    }

    bool contains_str(std::string s) {
        bool contains = false;
        for(int i = 0; i < fieldNames.size() && !contains; ++i) {
            contains |= contains_str(i, s);
        }
        return contains;
    }

  protected:
    LAbstractNode* left;
    LAbstractNode* right;
    int block_size;
};

class LCrossProductNode : public LAbstractNode{
  public:
    LCrossProductNode(LAbstractNode* left, LAbstractNode* right);
    ~LCrossProductNode();
};

class LJoinNode : public LAbstractNode {
  public:
    // offsets are defined as "TableName.AttributeName" so, ensure there is no duplicates
    LJoinNode(LAbstractNode* left, LAbstractNode* right, std::string attr1, std::string attr2);
    ~LJoinNode();   
    // attributes to perform equi-join on
    std::string attr1;
    std::string attr2;
    // maximum number of records permitted to present inside physical node
    int memorylimit;
};

class LProjectNode : public LAbstractNode{
  public:
    LProjectNode(LAbstractNode* child, std::vector<std::string> tokeep);
    ~LProjectNode();
    std::vector<std::string> attrs;
};

class LSelectNode : public LAbstractNode{
  public:
    LSelectNode(BaseTable& table, std::vector<Predicate> predicates, int block_size);
    // returns a reference to BaseTable
    BaseTable& GetBaseTable();
    // returns end status and next predicate (if exists)
    std::tuple<int, Predicate> GetNextPredicate();
    // resets predicate iterator
    void ResetIterator();
    ~LSelectNode();
  private:
    int iteratorpos;
    std::vector<Predicate> predicates;
    BaseTable table;
};

class LUnionNode : public LAbstractNode {
public:
    LUnionNode(LAbstractNode* left, LAbstractNode* right);
    ~LUnionNode();
};

class LUniqueNode : public LAbstractNode{
public:
    LUniqueNode(LAbstractNode* child);
    ~LUniqueNode();
};

// Physical node interface (result), should be used for automatic testing

class PResultNode{
protected:
    std::vector<std::vector<Value>> data;
    int pos;
    int block_pos; // for GetNext()

    // stats
    int in_records = 0;
    int out_records = 0;

    void load_next_block();
    void dump_buffer_to_file();

public:
    // used to get attribute info
    LAbstractNode* prototype;

    PResultNode(PResultNode* left, PResultNode* right, LAbstractNode* p);
    virtual ~PResultNode();
    // returns number of attributes
    virtual int GetAttrNum() = 0;
    // prints tree
    virtual void Print(int indent) = 0;
    // returns error status and data, if possible
    virtual std::tuple<ErrCode, std::vector<Value>> GetRecord();

    // stats
    int get_out_records() {
        return out_records;
    }
    int get_in_records() {
        return in_records;
    }

    PResultNode* left;
    PResultNode* right;
};

#endif // INTERFACE_H
