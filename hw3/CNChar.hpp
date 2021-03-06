#ifndef _CNCHAR_HPP_
#define _CNCHAR_HPP_
#include <stdint.h>
union CNChar
{
    private:
        uint8_t bytePair[2];
        uint16_t integer;

    public:
        bool operator < (const union CNChar & right) const 
        {
            return integer < right.integer;
        }

        bool operator == (const union CNChar & right) const
        {
            return integer == right.integer;
        }

        void set(const char *word);

        void toWord(char *word) const;

        void write() const;

        CNChar();
        CNChar(uint8_t b0, uint8_t b1);
        CNChar(const char *word);
};

const union CNChar zhuYinList[] = 
    {
        {0xa3, 0x74}, {0xa3, 0x75},
        {0xa3, 0x76}, {0xa3, 0x77},
        {0xa3, 0x78}, {0xa3, 0x79},
        {0xa3, 0x7a}, {0xa3, 0x7b},
        {0xa3, 0x7c}, {0xa3, 0x7d},
        {0xa3, 0x7e}, {0xa3, 0xa1},
        {0xa3, 0xa2}, {0xa3, 0xa3},
        {0xa3, 0xa4}, {0xa3, 0xa5},
        {0xa3, 0xa6}, {0xa3, 0xa7},
        {0xa3, 0xa8}, {0xa3, 0xa9},
        {0xa3, 0xaa}, {0xa3, 0xab},
        {0xa3, 0xac}, {0xa3, 0xad},
        {0xa3, 0xae}, {0xa3, 0xaf},
        {0xa3, 0xb0}, {0xa3, 0xb1},
        {0xa3, 0xb2}, {0xa3, 0xb3},
        {0xa3, 0xb4}, {0xa3, 0xb5},
        {0xa3, 0xb6}, {0xa3, 0xb7},
        {0xa3, 0xb8}, {0xa3, 0xb9},
        {0xa3, 0xba}, {0xa3, 0xbb},
        {0xa3, 0xbc}, {0xa3, 0xbd},
        {0xa3, 0xbe}, {0xa3, 0xbf},
    };
#endif