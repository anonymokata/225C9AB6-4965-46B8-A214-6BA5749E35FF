#ifndef rpntools_h_included
#define rpntools_h_included

#include "utils.h"  /* needed for validation_t */

void checkSanity(const char *str, validation_t validation_rule);
const char* RPNtoInfix(const char *str);
const char* getRPN(char[][SMBUFFER], int);

#endif
