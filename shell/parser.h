#ifndef SCPSHELL_PARSER_H
#define SCPSHELL_PARSER_H

#include "shell/utils.h"

int scpshellParsing_get_argc(const char* str);
void scpshellParsing_make_argv(int argc, char** argv, char* buffer);

#endif // SCPSHELL_PARSER_H
