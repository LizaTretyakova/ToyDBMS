#include <algorithm>
#include <cstddef>
#include <cstring>
#include <unordered_map>

#include "pjoinhashnode.h"
#include "pjoinnode.h"

void PJoinMergeSortNode::join_blocks(
        std::vector<std::vector<Value>> lres,
        std::vector<std::vector<Value>> rres,
        std::vector<std::vector<std::string>> ln,
        std::vector<std::vector<std::string>> rn,
        std::ptrdiff_t lpos,
        std::ptrdiff_t rpos,
        ValueType vt) {

    int lit = 0; // left iterator
    int rit = 0; // right iterator
    std::sort(lres.begin(), lres.end());
    std::sort(rres.begin(), rres.end());

    while(lit < lres.size() && rit < rres.size()) {
        bool join = vt == VT_INT ?
                ((int)lres[lit][lpos] == (int)rres[rit][rpos]) :
                ((std::string)lres[lit][lpos] == (std::string)rres[rit][rpos]);

        if(!join) {
            switch(vt) {
            case VT_INT:
                if((int)lres[lit][lpos] < (int)rres[rit][rpos]) {
                    ++lit;
                } else {
                    ++rit;
                }
                break;
            case VT_STRING:
                if(std::strcmp((string)lres[lit][lpos], (string)rres[rit][rpos]) < 0) {
                    ++lit;
                } else {
                    ++rit;
                }
                break;
            }
            continue;
        }

        std::vector<Value> tmp;
        for (int k = 0; k < ln.size(); k++){
            tmp.push_back(lres[lit][k]);
        }
        for (int k = 0; k < rn.size(); k++){
            if(k != rpos){
                tmp.push_back(rres[rit][k]);
            }
        }
        data.push_back(tmp);
    }
}
