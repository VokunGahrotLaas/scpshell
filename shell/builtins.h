#ifndef SCPSHELL_BUILTINS_H
#define SCPSHELL_BUILTINS_H

#include "shell/utils.h"

void scpshellBuiltins_build(struct scpshellEnv* env);
scpshellBuiltins_pair* scpshellBuiltins_search(struct scpshellEnv* env, char* cmd);

#endif // SCPSHELL_BUILTINS_H
