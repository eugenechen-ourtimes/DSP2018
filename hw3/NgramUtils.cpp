#include <assert.h>
#include "Ngram.h"
#include "NgramUtils.hpp"
#include "CNChar.hpp"

NgramUtils::NgramUtils(Vocab *voc, Ngram *lm)
{
	this->voc = voc;
	this->lm = lm;
	assert(voc != NULL && lm != NULL);
}

LogP NgramUtils::getUnigramProb(const union CNChar &ch)
{
    char w[4];
    ch.toWord(w);
    return getUnigramProb(w);
}

LogP NgramUtils::getInitialProb(const union CNChar &ch)
{
    char w[4];
    ch.toWord(w);
    return getInitialProb(w);
}

LogP NgramUtils::getBigramProb(const union CNChar &ch1, const union CNChar &ch2)
{
    char w1[4], w2[4];
    ch1.toWord(w1);
    ch2.toWord(w2);
    return getBigramProb(w1, w2);
}

LogP NgramUtils::getTrigramProb(const union CNChar &ch1, const union CNChar &ch2 , const union CNChar &ch3)
{   
    char w1[4], w2[4], w3[4];
    ch1.toWord(w1);
    ch2.toWord(w2);
    ch3.toWord(w3);
    return getTrigramProb(w1, w2, w3);
}

LogP NgramUtils::getUnigramProb(const char *w)
{
    VocabIndex wid = voc->getIndex(w);

    if (wid == Vocab_None)  //OOV
        wid = voc->getIndex(Vocab_Unknown);

    VocabIndex context[] = { Vocab_None };
    return lm->wordProb(wid, context);
}

LogP NgramUtils::getInitialProb(const char *w)
{
    VocabIndex wid = voc->getIndex(w);
    VocabIndex sid = voc->getIndex("<s>");

    if (wid == Vocab_None)  //OOV
        wid = voc->getIndex(Vocab_Unknown);

    VocabIndex context[] = { sid, Vocab_None };
    return lm->wordProb(wid, context);
}

LogP NgramUtils::getBigramProb(const char *w1, const char *w2)
{
    VocabIndex wid1 = voc->getIndex(w1);
    VocabIndex wid2 = voc->getIndex(w2);

    if (wid1 == Vocab_None)  //OOV
        wid1 = voc->getIndex(Vocab_Unknown);
    if (wid2 == Vocab_None)  //OOV
        wid2 = voc->getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm->wordProb( wid2, context);
}

LogP NgramUtils::getTrigramProb(const char *w1, const char *w2, const char *w3) 
{
    VocabIndex wid1 = voc->getIndex(w1);
    VocabIndex wid2 = voc->getIndex(w2);
    VocabIndex wid3 = voc->getIndex(w3);

    if (wid1 == Vocab_None)  //OOV
        wid1 = voc->getIndex(Vocab_Unknown);
    if (wid2 == Vocab_None)  //OOV
        wid2 = voc->getIndex(Vocab_Unknown);
    if (wid3 == Vocab_None)  //OOV
        wid3 = voc->getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid2, wid1, Vocab_None };
    return lm->wordProb( wid3, context );
}