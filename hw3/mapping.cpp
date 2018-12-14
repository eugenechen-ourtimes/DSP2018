#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>

#include "Constants.hpp"
#include "CNChar.hpp"

#define MAXL 1000
using namespace std;

FILE *fin;

void init_main(int argc, char *argv[]);
int findIndex(union CNChar zhuYin)
{
    for (int i = 0; i < ZhuYinCount; i++) {
        if (zhuYin == zhuYinList[i]) return i;
    }
    return -1;
}
int main(int argc, char *argv[])
{
	init_main(argc, argv);
	char line[MAXL];
	union CNChar zhuYin;

	set <union CNChar> charSet[ZhuYinCount];
	set <union CNChar> _charSet;
	const char *del = "/";

	while (fgets(line, MAXL, fin) != NULL) {
		union CNChar ch(line[0], line[1]);

		char *s = strtok(&line[3], del);
        while (s != NULL) {
            int index = findIndex(CNChar(s[0], s[1]));
            if (index < 0) {
                fprintf(stderr, "error\n");
                exit(-2);
            }

            charSet[index].insert(ch);

            s = strtok(NULL, del);
        }
	}
	fclose(fin);

    set <union CNChar>::iterator it;
    static union CNChar output;

	for (int i = 0; i < ZhuYinCount; i++) {
        if (charSet[i].empty()) continue;

        zhuYinList[i].write();
        for (it = charSet[i].begin(); it != charSet[i].end(); it++) {
            fwrite(&sp, 1, 1, stdout);
            it->write();
        }
        
        fwrite(&lf, 1, 1, stdout);

        for (it = charSet[i].begin(); it != charSet[i].end(); it++)
            _charSet.insert(*it);
        charSet[i].clear();
    }

    for (it = _charSet.begin(); it != _charSet.end(); it++) {
        it->write();
        fwrite(&sp, 1, 1, stdout);
        it->write();
        fwrite(&lf, 1, 1, stdout);
    }
}

void init_main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./[Program] [Big5-ZhuYin] > [ZhuYin-Big5]\n");
        exit(0);
    }

    if (( fin = fopen(argv[1], "r") ) == NULL) {
        perror(argv[1]);
        exit(-1);
    }
}