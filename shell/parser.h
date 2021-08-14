#ifndef SCPSHELL_PARSER_H
#define SCPSHELL_PARSER_H

#include "shell/utils.h"

size_t scpshellParsing_get_argc(const char* str);
void scpshellParsing_make_argv(size_t argc, char** argv, char* buffer);

#endif // SCPSHELL_PARSER_H
