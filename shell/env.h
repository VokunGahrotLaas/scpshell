#ifndef SCPSHELL_ENV_H
#define SCPSHELL_ENV_H

#include "shell/utils.h"
#include "shell/builtins.h"

struct scpshellEnv* scpshellEnv_new(int argc, char** argv, char** envp);
void scpshellEnv_delete(struct scpshellEnv* env);

static struct scpshellEnvType {
	struct scpshellEnv* (*new)(int argc, char** argv, char** envp);
	void (*delete)(struct scpshellEnv* env);
} const scpshellEnv = {
	.new = scpshellEnv_new,
	.delete = scpshellEnv_delete
};

#endif // SCPSHELL_ENV_H
