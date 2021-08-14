#include "shell/env.h"

static void scpshellEnv_init_path(struct scpshellEnv* env);
static void scpshellEnv_init_builtins(struct scpshellEnv* env);

static void scpshellEnv_init_path(struct scpshellEnv* env) {
	env->path = scpString_strdup(getenv("PATH"));
	if (!env->path) SCP_EXCEPTION(scpException_Exception, "no path variable in env");
	if (*env->path == '\0') SCP_EXCEPTION(scpException_Exception, "empty path variable in env");

	env->pathv = malloc(2 * sizeof(char*));
	*env->pathv = env->path;
	env->pathc = 1;
	for (size_t i = 0; env->path[i]; ++i) {
		if (env->path[i] == ':') {
			env->pathv = realloc(env->pathv, (size_t)(++env->pathc + 1) * sizeof(char*));
			env->pathv[env->pathc - 1] = env->path + i + 1;
			env->pathv[env->pathc] = NULL;
			env->path[i] = '\0';
		}
	}
}

static void scpshellEnv_init_builtins(struct scpshellEnv* env) {
	env->builtins = scpHashMap.new(scpHash_stringA, scpHash_stringB, scpCmp_string);
	scpshellBuiltins_build(env);
}

struct scpshellEnv* scpshellEnv_new(int argc, char** argv, char** envp) {
	struct scpshellEnv* env = malloc(sizeof(struct scpshellEnv));
	env->argc = argc;
	env->argv = argv;
	env->envp = envp;
	scpshellEnv_init_path(env);
	scpshellEnv_init_builtins(env);
	return env;
}

void scpshellEnv_delete(struct scpshellEnv* env) {
	free(env);
	free(env->path);
	free(env->pathv);
	scpHashMap.delete(env->builtins);
}
