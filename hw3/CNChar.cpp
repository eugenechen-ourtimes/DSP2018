#include "CNChar.hpp"
#include <stdio.h>
#include <assert.h>
void CNChar::toWord(char *word) const
{
    word[0] = bytePair[0];
    word[1] = bytePair[1];
    word[2] = '\0';
}

void CNChar::write() const
{
    fwrite(&integer, 1, 2, stdout);
}

void CNChar::set(const char* word)
{
    integer = 0;
    int i;
    for (i = 0; i < 2 && word[i] != '\0'; i++)
        bytePair[i] = word[i];
    assert(i == 2);
}

CNChar::CNChar()
{
    integer = 0;
}

CNChar::CNChar(uint8_t b0, uint8_t b1)
{
    bytePair[0] = b0;
    bytePair[1] = b1;
}

CNChar::CNChar(const char* word)
{
    integer = 0;
    int i;
    for (i = 0; i < 2 && word[i] != '\0'; i++)
        bytePair[i] = word[i];
    assert(i == 2);
}
