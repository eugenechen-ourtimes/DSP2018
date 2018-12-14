#ifndef _VITERBI_HPP_
#define _VITERBI_HPP_

#include <set>
#include <vector>
using namespace std;
#include "NgramUtils.hpp"
#include "CNChar.hpp"

class Viterbi {
    private:
        bool keepUnk;
        bool top240;
        int collectedZhuYin;
        union CNChar *zhuYin;
        set <union CNChar> *charSet;
        NgramUtils *utils;

        int findIndex(const union CNChar target);
        void showAns(vector <union CNChar> &ans);
        void keepTop240(vector <union CNChar> *candidates) const;
    public:
        Viterbi
        (
            int collectedZhuYin,
            union CNChar *zhuYin,
            set <union CNChar> *charSet,
            NgramUtils *utils,
            bool keepUnk,
            bool top240
        );

        void disambigOrder2(char *line);
        void disambigOrder3(char *line);
};

#endif