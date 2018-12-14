#include "Viterbi.hpp"
#include "Constants.hpp"
#define NEG_INDEX -1000

#include <stdio.h>
#include <algorithm>

using namespace std;

NgramUtils *_utils;

Viterbi::Viterbi
(
    int collectedZhuYin,
    union CNChar *zhuYin,
    set <union CNChar> *charSet,
    NgramUtils *utils,
    bool keepUnk,
    bool top240
)
{
    _utils = utils;
    this->collectedZhuYin = collectedZhuYin;
    this->zhuYin = zhuYin;
    this->charSet = charSet;
    this->utils = utils;
    this->keepUnk = keepUnk;
    this->top240 = top240;
    assert(zhuYin != NULL);
    assert(charSet != NULL);
    assert(utils != NULL);
    assert(collectedZhuYin <= ZhuYinCount);
}

int Viterbi::findIndex(const union CNChar target)
{
    for (int i = 1; i <= collectedZhuYin; i++) {
        if (target == zhuYin[i]) return i;
    }
    return 0;
}

void Viterbi::disambigOrder2(char *line)
{
    VocabString words[MAXCHARCOUNTPERLINE];
    unsigned len = Vocab::parseWords(line, words, MAXCHARCOUNTPERLINE);
    if (len == 0) {
        vector <union CNChar> ans(0);
        showAns(ans);
        return;
    }
    vector <LogP> delta[len];
    vector <int> path(len);
    vector <union CNChar> candidates[len];

    int index;
    for (int L = 0; L < len; L++) {
        index = findIndex(CNChar(words[L]));
        if (index == 0) {
            /* zhuYin */
            candidates[L].push_back(CNChar(words[L]));
        } else {
            /* Chinese Character */
            candidates[L].assign(charSet[index].begin(), charSet[index].end());
            if (top240) keepTop240(&candidates[L]);
        }            
    }

    for (int L = 0; L < len; L++)
        delta[L].resize(candidates[L].size());

    for (int i = 0; i < candidates[0].size(); i++) {
        delta[0][i] = utils->getInitialProb(candidates[0][i]);
    }

    LogP logProb;
    const LogP _maxP = (-100.0) * len;
    for (int L = 1; L < len; L++) {
        for (int i = 0; i < candidates[L].size(); i++) {
            LogP maxP = _maxP;
            for (int j = 0; j < candidates[L - 1].size(); j++) {
                logProb = delta[L - 1][j] + utils->getBigramProb
                (candidates[L - 1][j], candidates[L][i]);
                if (logProb > maxP) maxP = logProb;
            }
            delta[L][i] = maxP;
        }
    }

    int selected;
    selected = NEG_INDEX;
    LogP M = _maxP;
    for (int i = 0; i < candidates[len - 1].size(); i++) {
        if (delta[len - 1][i] > M) {
            M = delta[len - 1][i];
            selected = i;
        }
    }

    path[len - 1] = selected;

    for (int L = len - 2; L >= 0; L--) {
        LogP maxP = _maxP;
        selected = NEG_INDEX;
        for (int j = 0; j < candidates[L].size(); j++) {
            logProb = delta[L][j] + utils->getBigramProb
            (candidates[L][j], candidates[L + 1][path[L + 1]]);
            if (logProb > maxP) {
                maxP = logProb;
                selected = j;

            }
        }
        path[L] = selected;
    }

    vector <union CNChar> ans(len);
    for (int L = 0; L < len; L++)
        ans[L] = candidates[L][path[L]];
    showAns(ans);
}

static bool compare(const union CNChar &c1, const union CNChar &c2)
{
    LogP logProb1 = _utils->getUnigramProb(c1);
    LogP logProb2 = _utils->getUnigramProb(c2);
    return (logProb1 > logProb2);
}

void Viterbi::keepTop240(vector <union CNChar> *candidates) const
{
    if (candidates->size() <= 240) return;
    sort(candidates->begin(), candidates->end(), compare);
    candidates->resize(240);
}

void Viterbi::disambigOrder3(char *line)
{
    VocabString words[MAXCHARCOUNTPERLINE];
    unsigned len = Vocab::parseWords(line, words, MAXCHARCOUNTPERLINE);
    const LogP _maxP = (-100.0) * len;
    if (len == 0) {
        vector <union CNChar> ans(0);
        showAns(ans);
        return;
    }
    vector < vector<LogP> > delta[len];
    vector <int> path(len);
    vector <union CNChar> candidates[len];

    int index;
    for (int L = 0; L < len; L++) {
        index = findIndex(CNChar(words[L]));
        if (index == 0) {
            /* zhuYin */
            candidates[L].push_back(CNChar(words[L]));
        } else {
            /* Chinese Character */
            candidates[L].assign(charSet[index].begin(), charSet[index].end());
            if (top240) keepTop240(&candidates[L]);
        }            
    }

    if (len == 1) {
        int selectedk = NEG_INDEX;
        LogP maxP = _maxP;
        LogP p;
        for (int k = 0; k < candidates[0].size(); k++) {
            p = utils->getInitialProb(candidates[0][k]);
            if (p > maxP) {
                maxP = p;
                selectedk = k;
            }
        }

        vector<union CNChar> ans(1);
        ans[0] = candidates[0][selectedk];
        showAns(ans);
        return;
    }

    for (int L = 1; L < len; L++) {
        delta[L].resize(candidates[L].size());
        for (int _L = 0; _L < delta[L].size(); _L++)
            delta[L][_L].resize(candidates[L - 1].size());
    }


    for (int i = 0; i < candidates[1].size(); i++)
        for (int j = 0; j < candidates[0].size(); j++)
            delta[1][i][j] =
            utils->getBigramProb(candidates[0][j], candidates[1][i]) +
            utils->getInitialProb(candidates[0][j]);

    LogP logProb;

    for (int L = 2; L < len; L++) {
        for (int i = 0; i < candidates[L].size(); i++)
            for (int j = 0; j < candidates[L - 1].size(); j++) {
                LogP maxP = _maxP;
                for (int k = 0; k < candidates[L - 2].size(); k++) {
                    logProb =
                    delta[L - 1][j][k] + utils->getTrigramProb
                    (candidates[L - 2][k], candidates[L - 1][j], candidates[L][i]);
                    if (logProb > maxP) maxP = logProb;
                }
                delta[L][i][j] = maxP;
            }
    }

    int selectedi, selectedj, selectedk;

    LogP M = _maxP;

    selectedi = NEG_INDEX; selectedj = NEG_INDEX;
    for (int i = 0; i < candidates[len - 1].size(); i++)
        for (int j = 0; j < candidates[len - 2].size(); j++) {
            if (delta[len - 1][i][j] > M) {
                M = delta[len - 1][i][j];
                selectedi = i;
                selectedj = j;
            }
        }

    path[len - 1] = selectedi;
    path[len - 2] = selectedj;

    for (int L = len - 3; L >= 1; L--) {
        selectedk = NEG_INDEX;
        LogP maxP = _maxP;
        int i = path[L + 2]; int j = path[L + 1];
        for (int k = 0; k < candidates[L].size(); k++) {
            logProb = delta[L + 1][j][k] + utils->getTrigramProb
            (candidates[L][k], candidates[L + 1][j], candidates[L + 2][i]);
            if (logProb > maxP) {
                maxP = logProb;
                selectedk = k;
            }
        }
        path[L] = selectedk;
    }

    int i = path[1];

    M = _maxP;
    selectedj = NEG_INDEX;
    for (int j = 0; j < candidates[0].size(); j++) {
        logProb =
        utils->getInitialProb(candidates[0][j]) +
        utils->getBigramProb(candidates[0][j], candidates[1][i]);
        if (logProb > M) {
            M = logProb;
            selectedj = j;
        }
    }

    path[0] = selectedj;

    vector <union CNChar> ans(len);
    for (int L = 0; L < len; L++)
        ans[L] = candidates[L][path[L]];
    showAns(ans);
}

void Viterbi::showAns(vector <union CNChar> &ans)
{
    const char *start = "<s>";
    const char *end = "</s>";
    const size_t startL = 3;
    const size_t endL = 4;

    fwrite(start, 1, startL, stdout);
    fwrite(&sp, 1, 1, stdout);

    if (keepUnk) {
        for (int L = 0; L < ans.size(); L++) {
            ans[L].write();
            fwrite(&sp, 1, 1, stdout);
        }
    } else {
        const char *unk = "<unk>";
        const size_t unkL = 5;
        for (int L = 0; L < ans.size(); L++) {
            if (charSet[0].find(ans[L]) == charSet[0].end()) {
                fwrite(unk, 1, unkL, stdout);
            } else {
                ans[L].write();
            }
            fwrite(&sp, 1, 1, stdout);
        }
    }

    fwrite(end, 1, endL, stdout);
    fwrite(&lf, 1, 1, stdout);
}