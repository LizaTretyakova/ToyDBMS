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
            PJoinNode(left, right, p) {}
        ~PJoinHashNode();
    private:
        void join_blocks_int(
                std::vector<std::vector<Value>> lres,
                std::vector<std::vector<Value>> rres,
                std::vector<std::vector<std::string>> ln,
                std::vector<std::vector<std::string>> rn,
                std::ptrdiff_t lpos,
                std::ptrdiff_t rpos);
        void join_blocks_str(
                std::vector<std::vector<Value>> lres,
                std::vector<std::vector<Value>> rres,
                std::vector<std::vector<std::string>> ln,
                std::vector<std::vector<std::string>> rn,
                std::ptrdiff_t lpos,
                std::ptrdiff_t rpos);
};

#endif // PJOINHASHNODE_H
