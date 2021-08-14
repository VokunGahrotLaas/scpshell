#include "shell/builtins.h"

static int scpshellBuiltins_func_ping(struct scpshellEnv* env);
static int scpshellBuiltins_func_args(struct scpshellEnv* env);
static int scpshellBuiltins_func_exit(struct scpshellEnv* env);
static int scpshellBuiltins_func_cd(struct scpshellEnv* env);
static int scpshellBuiltins_func_shsleep(struct scpshellEnv* env);

static scpshellBuiltins_pair scpshellBuiltins_pairs[] = {
	{ "ping", scpshellBuiltins_func_ping },
	{ "args", scpshellBuiltins_func_args },
	{ "exit", scpshellBuiltins_func_exit },
	{ "cd", scpshellBuiltins_func_cd },
	{ "shsleep", scpshellBuiltins_func_shsleep }
};

void scpshellBuiltins_build(struct scpshellEnv* env) {
	for (size_t i = 0; i < sizeof scpshellBuiltins_pairs / sizeof *scpshellBuiltins_pairs; ++i)
		scpHashMap.insert(env->builtins, scpshellBuiltins_pairs[i].key, scpshellBuiltins_pairs + i);
 }

scpshellBuiltins_pair* scpshellBuiltins_search(struct scpshellEnv* env, char* cmd) {
	return (scpshellBuiltins_pair*)scpHashMap.search(env->builtins, cmd);
}

int scpshellBuiltins_func_ping(scpAttribute_unused struct scpshellEnv* env) {
	fputs("pong!\n", stdout);
	return EXIT_SUCCESS;
}

static int scpshellBuiltins_func_args(struct scpshellEnv* env) {
	printf("argc: %i\n", env->sub_argc);
	fputs("argv: [", stdout);
	if (env->sub_argc > 0)
		printf(" '%s'", *env->sub_argv);
	for (int i = 1; i < env->sub_argc; ++i)
		printf(", '%s'", env->sub_argv[i]);
	if (env->sub_argc > 0)
		fputc(' ', stdout);
	fputs("]\n", stdout);
	return EXIT_SUCCESS;
}

static int scpshellBuiltins_func_exit(scpAttribute_unused struct scpshellEnv* env) {
	exit(EXIT_SUCCESS);
}

static int scpshellBuiltins_func_cd(struct scpshellEnv* env) {
	if (env->sub_argc < 2) {
		fputs("cd: not yet dude\n", stdout);
		return EXIT_SUCCESS;
	}
	if (env->sub_argc > 2) {
		fputs("cd: too many arguments\n", stdout);
		return EXIT_FAILURE;
	}
	if (chdir(env->sub_argv[1]) == 0)
		return EXIT_SUCCESS;
	switch (errno) {
		case EINTR:
			break;
		case EACCES:
			fputs("cd: search permission has been denied\n", stderr);
			break;
		case ELOOP:
			fputs("cd: too many symbolic links were encountered\n", stderr);
			break;
		case ENAMETOOLONG:
			fputs("cd: path is too large (PATH_MAX: "SCP_TO_STRING(PATH_MAX)")\n", stderr);
			break;
		case ENOENT:
			fprintf(stderr, "cd: no such file or directory: %s\n", env->sub_argv[1]);
			break;
		case ENOTDIR:
			fprintf(stderr, "cd: not a directory: %s\n", env->sub_argv[1]);
			break;
		default:
			fputs("cd: unknown error\n", stderr);
			break;
	}
	return errno;
}

static int scpshellBuiltins_func_shsleep(struct scpshellEnv* env) {
	if (env->sub_argc < 2) {
		fputs("sleep: too few arguments\n", stdout);
		return EXIT_FAILURE;
	}
	for (int i = 1; i < env->sub_argc; ++i) {
		unsigned to_sleep;
		if (sscanf(env->sub_argv[i], "%u", &to_sleep) != 1) {
			fprintf(stderr, "sleep: invalid argument: %s", env->sub_argv[i]);
			return EXIT_FAILURE;
		}
		if (sleep(to_sleep) != 0)
			return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
