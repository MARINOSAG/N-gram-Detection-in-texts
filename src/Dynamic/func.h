#ifndef func
#define func

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int sentence_split(char *sentence, char **&words);
char* delete_duplicate(char* sentence);
int is_empty_string(char *sentence);
#endif