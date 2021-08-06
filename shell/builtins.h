#ifndef SCPSHELL_BUILTINS_H
#define SCPSHELL_BUILTINS_H

#include "shell/utils.h"

typedef int (*scpshellBuiltins_func)(size_t argc, char** argv);

typedef struct scpshellBuiltins_pair {
	const char* key;
	const scpshellBuiltins_func value;
} scpshellBuiltins_pair;

scpAttribute_malloc
struct scpHashMap* scpshellBuiltins_build_hashmap(void);

#endif // SCPSHELL_BUILTINS_H
