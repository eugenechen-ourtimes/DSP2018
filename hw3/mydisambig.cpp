#include <stdio.h>
#include <stdlib.h>
#include <set>
#include "Ngram.h"
#include "option.h"
#include "CNChar.hpp"
#include "NgramUtils.hpp"
#include "Viterbi.hpp"
#include "Constants.hpp"

using namespace std;

static unsigned ngramOrder = 0;
static char *lmFileName = NULL;
static char *mapFileName = NULL;
static char *textFileName = NULL;
static bool keepUnk = false;
static bool top240 = true;

static Option options[] = {
    { OPT_STRING, "text", &textFileName, "text file to disambiguate" },
    { OPT_STRING, "map", &mapFileName, "mapping from observation to candidates" },
    { OPT_STRING, "lm", &lmFileName, "language model for candidates" },
    { OPT_UINT, "order", &ngramOrder, "ngram order" },
    { OPT_TRUE, "keep-unk", &keepUnk, "show OOV as <unk> or not" },
    { OPT_FALSE, "consider-rare", &top240, "if not set, only consider top 240 characters in candidate list" }
};

void init_main(int argc, char *argv[]);
void disambig(Viterbi &viterbi, char *line);

/* Usage: ./[Program]   -text   [inputFile]
                        -map    [ZhuYin-Big5]
                        -lm     [languageModel]
                        -order  [ngramOrder]
                        ##  -keep-unk
                        ##  -consider-rare
*/

int main(int argc, char *argv[])
{
    init_main(argc, argv);
    Vocab voc;
    Ngram lm(voc, ngramOrder);
    
    /* read language model */
    File lmFile(lmFileName, "r");
    lm.read(lmFile);
    lmFile.close();

    /* get sets of Chinese character from map */
    char *line;    
    File mapping(mapFileName, "r");

    set <union CNChar> charSet[ZhuYinCount + 1];

    union CNChar zhuYin[ZhuYinCount + 1];
    int collectedZhuYin = 1;

    while ((line = mapping.getline()) != NULL) {
        VocabString words[MaxMappingSize];
        unsigned len = Vocab::parseWords(line, words, MaxMappingSize);
        if (len == 1) continue;
        if (len == 2 && strcmp(words[0], words[1]) == 0) {
            charSet[0].insert(CNChar(words[0]));
        } else {
            zhuYin[collectedZhuYin].set(words[0]);
            for (int i = 1; i < len; i++) {
                charSet[collectedZhuYin].insert(CNChar(words[i]));
            }
            collectedZhuYin++;
        }
    }

    collectedZhuYin--;
    mapping.close();
    NgramUtils utils(&voc, &lm);
    Viterbi viterbi(collectedZhuYin, zhuYin, charSet, &utils, keepUnk, top240);

    File textFile(textFileName, "r");

    while ((line = textFile.getline()) != NULL) {
        disambig(viterbi, line);
    }

    textFile.close();
}

void disambig(Viterbi &viterbi, char *line)
{
    if (ngramOrder == 2) {
        viterbi.disambigOrder2(line);
        return;
    }

    viterbi.disambigOrder3(line);
}

void init_main(int argc, char *argv[])
{
    Opt_Parse(argc, argv, options, 6, 0);

    if (textFileName == NULL) {
        fprintf(stderr, "text file unspecified\n");
        exit(-1);
    }

    if (mapFileName == NULL) {
        fprintf(stderr, "mapping file unspecified\n");
        exit(-1);
    }

    if (lmFileName == NULL) {
        fprintf(stderr, "language model unspecified\n");
        exit(-1);
    }

    if (ngramOrder == 0) {
        fprintf(stderr, "ngram-order unspecified\n");
        exit(-1);
    }

    if (ngramOrder == 1 || ngramOrder > 3) {
        fprintf(stderr, "ngram-order %d unsupported\n", ngramOrder);
        exit(-1);
    }
}