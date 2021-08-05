#ifndef SCPSHELL_BUILTINS_H
#define SCPSHELL_BUILTINS_H

#include "scpshell/utils.h"

typedef int (*scpshellBuiltins_func)(size_t argc, char** argv);

typedef struct scpshellBuiltins_pair {
	const char* key;
	const scpshellBuiltins_func value;
} scpshellBuiltins_pair;

scpAttribute_malloc
struct scpHashMap* scpshellBuiltins_build_hashmap(void);

static int scpshellBuiltins_func_ping(size_t argc, char** argv);
static int scpshellBuiltins_func_args(size_t argc, char** argv);
static int scpshellBuiltins_func_exit(size_t argc, char** argv);
static int scpshellBuiltins_func_cd(size_t argc, char** argv);
static int scpshellBuiltins_func_shsleep(size_t argc, char** argv);

static scpshellBuiltins_pair scpshellBuiltins_pairs[] = {
	{ "ping", scpshellBuiltins_func_ping },
	{ "args", scpshellBuiltins_func_args },
	{ "exit", scpshellBuiltins_func_exit },
	{ "cd", scpshellBuiltins_func_cd },
	{ "shsleep", scpshellBuiltins_func_shsleep }
};

struct scpHashMap* scpshellBuiltins_build_hashmap(void) {
	struct scpHashMap* hashmap = scpHashMap.new(scpHash_stringA, scpHash_stringB, scpCmp_string);
	for (size_t i = 0; i < sizeof scpshellBuiltins_pairs / sizeof *scpshellBuiltins_pairs; ++i)
		scpHashMap.insert(hashmap, scpshellBuiltins_pairs[i].key, scpshellBuiltins_pairs + i);
	return hashmap;
}

int scpshellBuiltins_func_ping(scpAttribute_unused size_t argc, scpAttribute_unused char** argv) {
	fputs("pong!\n", stdout);
	return EXIT_SUCCESS;
}

static int scpshellBuiltins_func_args(size_t argc, char** argv) {
	printf("argc: %zu\n", argc);
	fputs("argv: [", stdout);
	if (argc > 0)
		printf(" '%s'", *argv);
	for (size_t i = 1; i < argc; ++i)
		printf(", '%s'", argv[i]);
	if (argc > 0)
		fputc(' ', stdout);
	fputs("]\n", stdout);
	return EXIT_SUCCESS;
}

static int scpshellBuiltins_func_exit(scpAttribute_unused size_t argc, scpAttribute_unused char** argv) {
	exit(EXIT_SUCCESS);
}

static int scpshellBuiltins_func_cd(size_t argc, char** argv) {
	if (argc < 2) {
		fputs("cd: not yet dude\n", stdout);
		return EXIT_SUCCESS;
	}
	if (argc > 2) {
		fputs("cd: too many arguments\n", stdout);
		return EXIT_FAILURE;
	}
	if (chdir(argv[1]) == 0)
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
			fprintf(stderr, "cd: no such file or directory: %s\n", argv[1]);
			break;
		case ENOTDIR:
			fprintf(stderr, "cd: not a directory: %s\n", argv[1]);
			break;
		default:
			fputs("cd: unknown error\n", stderr);
			break;
	}
	return errno;
}

static int scpshellBuiltins_func_shsleep(size_t argc, char** argv) {
	if (argc < 2) {
		fputs("sleep: too few arguments\n", stdout);
		return EXIT_FAILURE;
	}
	for (size_t i = 1; i < argc; ++i) {
		unsigned to_sleep;
		if (sscanf(argv[i], "%u", &to_sleep) != 1) {
			fprintf(stderr, "sleep: invalid argument: %s", argv[i]);
			return EXIT_FAILURE;
		}
		if (sleep(to_sleep) != 0)
			return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

#endif // SCPSHELL_BUILTINS_H
