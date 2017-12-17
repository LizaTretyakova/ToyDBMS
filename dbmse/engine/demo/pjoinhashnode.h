#ifndef PJOINHASHNODE_H
#define PJOINHASHNODE_H

#include <unordered_map>
#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"
#include "pjoinnode.h"

class PJoinHashNode : public PJoinNode {
    public:
        PJoinHashNode(PGetNextNode* left, PGetNextNode* right, LAbstractNode* p):
            PJoinNode(left, right, p) {
            if(vt == VT_INT) {
                for(int i = 0; i < left_data.size(); ++i) {
                    int_table[left_data[i][lpos]].push_back(i);
                }
            } else {
                for(int i = 0; i < left_data.size(); ++i) {
                    string_table[left_data[i][lpos]].push_back(i);
                }
            }
        }

        ~PJoinHashNode();
        virtual std::vector<std::vector<Value>> join_blocks();
        virtual void Print(int indent);
    private:
        std::unordered_map<int, std::vector<int>> int_table;
        std::unordered_map<std::string, std::vector<int>> string_table;
};

#endif // PJOINHASHNODE_H
