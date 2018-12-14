#ifndef _NGRAMUTILS_HPP_
#define _NGRAMUTILS_HPP_
#include "Ngram.h"
class NgramUtils {
    private:
        Vocab *voc = NULL;
        Ngram *lm = NULL;
    public:
        NgramUtils(Vocab *voc, Ngram *lm);
        LogP getUnigramProb(const union CNChar &ch);
        LogP getInitialProb(const union CNChar &ch);
        LogP getBigramProb(const union CNChar &ch1, const union CNChar &ch2);
        LogP getTrigramProb(const union CNChar &ch1, const union CNChar &ch2, const union CNChar &ch3);

        LogP getUnigramProb(const char *w); /* Get P(W) */
        LogP getInitialProb(const char *w); /* Get P(W | <s>) */
        LogP getBigramProb(const char *w1, const char *w2); /* Get P(W2 | W1) */
        LogP getTrigramProb(const char *w1, const char *w2, const char *w3); /* Get P(W3 | W1, W2) */
};
#endif