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

#include <cassert>
#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <tuple>
#include "../interface/interface.h"
#include "../interface/basics.h"
#include "pselectnode.h"
#include "pjoinhashnode.h"
#include "pjoinmergesortnode.h"
#include "pjoinnode.h"
#include "pcrossproductnode.h"
#include "pprojectnode.h"
#include "punionnode.h"
#include "puniquenode.h"

// Here be rewriter and optimizer
PResultNode* QueryFactory(LAbstractNode* node){
    if (dynamic_cast<LSelectNode*>(node) != NULL){
        std::cerr << "[QueryFactory] P/LSelectNode" << std::endl;
        std::cerr << node << std::endl;
        return new PSelectNode(node);
//        return new PSelectNode((LSelectNode*)node);
    } else if (dynamic_cast<LJoinNode*>(node) != NULL){
        std::cerr << "[QueryFactory] P/LJoinNode" << std::endl;

        PGetNextNode* lres = (PGetNextNode*)QueryFactory(node->GetLeft());
        PGetNextNode* rres = (PGetNextNode*)QueryFactory(node->GetRight());

        return new PJoinNode(lres, rres, node);
    } else if (dynamic_cast<LCrossProductNode*>(node) != NULL) {
        std::cerr << "[QueryFactory] P/LCrossProductNode" << std::endl;

        PGetNextNode* lres = (PGetNextNode*)QueryFactory(node->GetLeft());
        PGetNextNode* rres = (PGetNextNode*)QueryFactory(node->GetRight());

        return new PCrossProductNode(lres, rres, node);
    } else if (dynamic_cast<LProjectNode*>(node) != NULL) {
        std::cerr << "[QueryFactory] P/LProjectNode" << std::endl;

        PGetNextNode* cres = (PGetNextNode*)QueryFactory(node->GetLeft());

        return new PProjectNode(cres, node);
    } else if (dynamic_cast<LUnionNode*>(node) != NULL) {
        std::cerr << "[QueryFactory] P/LUnionNode" << std::endl;

        PGetNextNode* lres = (PGetNextNode*)QueryFactory(node->GetLeft());
        PGetNextNode* rres = (PGetNextNode*)QueryFactory(node->GetRight());

        return new PUnionNode(lres, rres, node);
    } else if (dynamic_cast<LUniqueNode*>(node) != NULL) {
        std::cerr << "[QueryFactory] P/LUniqueNode" << std::endl;

        PGetNextNode* cres = (PGetNextNode*)QueryFactory(node->GetLeft());

        return new PUniqueNode(cres, node);
    }

    std::cerr << "[QueryFactory] Node not found" << std::endl;
    return NULL;
}

std::vector<std::vector<Value>> ExecuteQuery(PResultNode* query){
    PGetNextNode* p = (PGetNextNode*)query;
    std::vector<std::vector<Value>> res;
    std::pair<bool, std::vector<std::vector<Value>>> tmp = p->GetNext();
    while(tmp.first) {
        res.insert(res.end(), tmp.second.begin(), tmp.second.end());

        for(int i = 0; i < tmp.second.size(); ++i) {
            for(int j = 0; j < tmp.second[i].size(); ++j) {
                if(tmp.second[i][j].vtype == VT_INT) {
                    std::cout << tmp.second[i][j].vint << "\t";
                } else {
                    std::cout << tmp.second[i][j].vstr << "\t";
                }
            }
            std::cout << std::endl;
        }
        tmp = p->GetNext();
    }
    return res;
}

bool are_equal(std::vector<std::vector<Value>> exp, std::vector<std::vector<Value>> act) {
    if(exp.size() != act.size()) {
        return false;
    }

    for(int i = 0; i < exp.size(); ++i) {
        if(exp[i].size() != act[i].size()) {
            return false;
        }
        for(int j = 0; j < exp[i].size(); ++j) {
            if(exp[i][j].vtype != act[i][j].vtype) {
                return false;
            }
            switch(exp[i][j].vtype) {
            case VT_INT:
                if(exp[i][j].vint != act[i][j].vint) {
                    return false;
                }
                break;
            case VT_STRING:
                if(exp[i][j].vstr != act[i][j].vstr) {
                    return false;
                }
                break;
            }
        }
    }

    return true;
}

void print_stats(PResultNode* q, int indent) {
    if(q->left) {
        print_stats((PResultNode*)q->left, indent + 2);
    }

    for(int i = 0; i < indent; ++i) {
        std::cout << " ";
    }
    std::cout << "in: " << q->get_in_records() << " "
              << "out: " << q->get_out_records() << std::endl;

    if(q->right) {
        print_stats((PResultNode*)q->right, indent + 2);
    }
}

void test_it_with_fire(LAbstractNode* n, std::string s, std::vector<std::vector<Value>> exp) {
    std::cout << std::endl << s << std::endl;
    std::cerr << "[Test] before building query" << std::endl;
    PResultNode* q = QueryFactory(n);
    std::cerr << "[Test] query built";
    q->Print(0);
    std::vector<std::vector<Value>> act = ExecuteQuery(q);
    assert(are_equal(exp, act));
    std::cout << "\033[32mOK\033[0m" << std::endl;
    print_stats(q, 0);
    delete q;
}

int main(){
    std::cout << "Starting demo" << std::endl;

    {
        BaseTable bt1 = BaseTable("table1");
        std::cout << bt1;
        Predicate p1(PT_EQUALS, VT_INT, 3, 4, "");
        Predicate p2(PT_GREATERTHAN, VT_INT, 0, 3, "");
        LAbstractNode* n1 = new LSelectNode(bt1, {p1, p2}, 100);

        std::vector<std::vector<Value>> exp = {
            {Value(0), Value("cero"), Value(100), Value(4)}
        };

        test_it_with_fire(n1, "SELECT * WHERE groups == 4 AND id < 3", exp);
        delete n1;
    }

    {
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");
        std::cout << bt1;
        std::cout << bt2;
        LAbstractNode* n1 = new LSelectNode(bt1, {}, 100);
        LAbstractNode* n2 = new LSelectNode(bt2, {}, 100);
        LJoinNode* n3 = new LJoinNode(n1, n2, "table1.id", "table2.id2");
        PResultNode* q1 = QueryFactory(n3);

        q1->Print(0);
        std::vector<std::vector<Value>> act = ExecuteQuery(q1);
        std::vector<std::vector<Value>> exp = {
            {Value("uno"), Value(55), Value(1), Value("one"), Value(1)},
            {Value("dos"), Value(25), Value(2), Value("two"), Value(2)},
            {Value("tres"), Value(23), Value(4), Value("three"), Value(3)},
            {Value("cuatro"), Value(33), Value(41), Value("four"), Value(4)},
            {Value("cinco"), Value(56), Value(12), Value("five"), Value(5)}
        };
        assert(are_equal(exp, act));
        std::cout << "\033[32mOK\033[0m" << std::endl;

        delete n3;
        delete q1;
    }

    {
        BaseTable bt1 = BaseTable("table1");
        LAbstractNode* n1 = new LSelectNode(bt1, {}, 100);
        LAbstractNode* n2 = new LProjectNode(n1, {"table1.id", "table1.frequency"});
        std::vector<std::vector<Value>> exp = {
            {Value(0), Value(100)},
            {Value(1), Value(55)},
            {Value(2), Value(25)},
            {Value(3), Value(23)},
            {Value(4), Value(33)},
            {Value(5), Value(56)}
        };
        test_it_with_fire(n2, "SELECT id, frequency FROM table1", exp);
        delete n2;
    }

    {
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");

        LAbstractNode* select1 = new LSelectNode(bt1, {}, 100);
        LAbstractNode* select2 = new LSelectNode(bt2, {}, 100);
        LAbstractNode* crossp = new LCrossProductNode(select1, select2);
        LAbstractNode* proj = new LProjectNode(crossp, {"table1.id", "table2.id2", "table2.type2"});

        std::vector<std::vector<Value>> exp = {
            {Value(0), Value(5), Value("five")},
            {Value(0), Value(4), Value("four")},
            {Value(0), Value(3), Value("three")},
            {Value(0), Value(2), Value("two")},
            {Value(0), Value(1), Value("one")},
            {Value(1), Value(5), Value("five")},
            {Value(1), Value(4), Value("four")},
            {Value(1), Value(3), Value("three")},
            {Value(1), Value(2), Value("two")},
            {Value(1), Value(1), Value("one")},
            {Value(2), Value(5), Value("five")},
            {Value(2), Value(4), Value("four")},
            {Value(2), Value(3), Value("three")},
            {Value(2), Value(2), Value("two")},
            {Value(2), Value(1), Value("one")},
            {Value(3), Value(5), Value("five")},
            {Value(3), Value(4), Value("four")},
            {Value(3), Value(3), Value("three")},
            {Value(3), Value(2), Value("two")},
            {Value(3), Value(1), Value("one")},
            {Value(4), Value(5), Value("five")},
            {Value(4), Value(4), Value("four")},
            {Value(4), Value(3), Value("three")},
            {Value(4), Value(2), Value("two")},
            {Value(4), Value(1), Value("one")},
            {Value(5), Value(5), Value("five")},
            {Value(5), Value(4), Value("four")},
            {Value(5), Value(3), Value("three")},
            {Value(5), Value(2), Value("two")},
            {Value(5), Value(1), Value("one")}
        };
        test_it_with_fire(proj,
            "(SELECT id FROM table1) x (SELECT id2 FROM table2)", exp);
        delete proj;
    }

    {
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");

        LAbstractNode* select1 = new LSelectNode(bt1, {}, 100);
        LAbstractNode* select2 = new LSelectNode(bt2, {}, 100);
        LAbstractNode* crossp = new LCrossProductNode(select1, select2);
        LAbstractNode* proj = new LProjectNode(crossp, {"table1.id", "table2.id2", "table2.type2"});
        LAbstractNode* uniq = new LUniqueNode(proj);

        std::vector<std::vector<Value>> exp = {
            {Value(0), Value(5), Value("five")},
            {Value(0), Value(4), Value("four")},
            {Value(0), Value(3), Value("three")},
            {Value(0), Value(2), Value("two")},
            {Value(0), Value(1), Value("one")},
            {Value(1), Value(5), Value("five")},
            {Value(1), Value(4), Value("four")},
            {Value(1), Value(3), Value("three")},
            {Value(1), Value(2), Value("two")},
            {Value(1), Value(1), Value("one")},
            {Value(2), Value(5), Value("five")},
            {Value(2), Value(4), Value("four")},
            {Value(2), Value(3), Value("three")},
            {Value(2), Value(2), Value("two")},
            {Value(2), Value(1), Value("one")},
            {Value(3), Value(5), Value("five")},
            {Value(3), Value(4), Value("four")},
            {Value(3), Value(3), Value("three")},
            {Value(3), Value(2), Value("two")},
            {Value(3), Value(1), Value("one")},
            {Value(4), Value(5), Value("five")},
            {Value(4), Value(4), Value("four")},
            {Value(4), Value(3), Value("three")},
            {Value(4), Value(2), Value("two")},
            {Value(4), Value(1), Value("one")},
            {Value(5), Value(5), Value("five")},
            {Value(5), Value(4), Value("four")},
            {Value(5), Value(3), Value("three")},
            {Value(5), Value(2), Value("two")},
            {Value(5), Value(1), Value("one")}
        };
        test_it_with_fire(uniq,
            "UNIQUE (SELECT id FROM table1) x (SELECT id2 FROM table2)", exp);
        delete uniq;
    }

    {
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");

        Predicate p1(PT_GREATERTHAN, VT_INT, 0, 5, "");
        Predicate p2(PT_GREATERTHAN, VT_STRING, 1, 0, "three");

        LAbstractNode* select1 = new LSelectNode(bt1, {p1}, 100);
        LAbstractNode* select2 = new LSelectNode(bt2, {p2}, 100);
        LAbstractNode* join = new LJoinNode(select1, select2, "table1.id", "table2.id2");
        LAbstractNode* proj = new LProjectNode(join, {"table1.id", "table2.type2"});

        std::vector<std::vector<Value>> exp = {
            {Value("one"), Value(1)},
            {Value("four"), Value(4)}
        };
        test_it_with_fire(proj,
            "SELECT id, id2, type2\n"
            "FROM table1 JOIN table2\n"
            "ON id == id2\n"
            "WHERE id < 5 AND type2 < 'three'", exp);
        delete proj;
    }

    {
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");

        Predicate p1(PT_EQUALS, VT_INT, 3, 4, "");
        Predicate p2(PT_GREATERTHAN, VT_INT, 0, 3, "");

        LAbstractNode* select1 = new LSelectNode(bt1, {p1, p2}, 100);
        LAbstractNode* select2 = new LSelectNode(bt2, {}, 100);
        LAbstractNode* select3 = new LSelectNode(bt1, {}, 100);
        LAbstractNode* crossp = new LCrossProductNode(select1, select2);
        LAbstractNode* join = new LJoinNode(crossp, select3, "table1.id", "table1.id");

        std::vector<std::vector<Value>> exp = {
            {Value("cero"), Value(100), Value(4), Value(5), Value("five"), Value("cero"), Value(100), Value(4), Value(0)},
            {Value("cero"), Value(100), Value(4), Value(4), Value("four"), Value("cero"), Value(100), Value(4), Value(0)},
            {Value("cero"), Value(100), Value(4), Value(3), Value("three"), Value("cero"), Value(100), Value(4), Value(0)},
            {Value("cero"), Value(100), Value(4), Value(2), Value("two"), Value("cero"), Value(100), Value(4), Value(0)},
            {Value("cero"), Value(100), Value(4), Value(1), Value("one"), Value("cero"), Value(100), Value(4), Value(0)}
        };
        test_it_with_fire(join,
            "(SELECT * FROM table1 WHERE groups == 4 AND id < 3)"
            "\nx\n"
            "(SELECT * FROM table2)\n"
            "JOIN table1 ON id == id\n", exp);
        delete join;
    }

    {
        BaseTable bt1 = BaseTable("table1");
        BaseTable bt2 = BaseTable("table2");

        Predicate p1(PT_EQUALS, VT_INT, 3, 4, "");
        Predicate p2(PT_GREATERTHAN, VT_INT, 0, 3, "");

        LAbstractNode* select1 = new LSelectNode(bt1, {p1, p2}, 100);
        LAbstractNode* select2 = new LSelectNode(bt2, {}, 100);
        LAbstractNode* select3 = new LSelectNode(bt1, {}, 100);
        LAbstractNode* crossp = new LCrossProductNode(select1, select2);
        LAbstractNode* join = new LJoinNode(crossp, select3, "table1.id", "table1.id");
        LAbstractNode* proj = new LProjectNode(join, {"table1.description", "table1.frequency", "table1.groups"});
        LAbstractNode* uniq = new LUniqueNode(proj);

        std::vector<std::vector<Value>> exp = {
            {Value("cero"), Value(100), Value(4), Value("cero"), Value(100), Value(4)}
        };
        test_it_with_fire(uniq,
            "(SELECT * FROM table1 WHERE groups == 4 AND id < 3)"
            "\nx\n"
            "(SELECT * FROM table2)\n"
            "JOIN table1 ON id == id\n", exp);
        delete uniq;
    }
}
