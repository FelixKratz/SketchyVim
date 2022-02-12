#pragma once
#include "Cocoa/Cocoa.h"
#include <sys/stat.h>
#include "env_vars.h"

#define FORK_TIMEOUT 60

char* string_copy(char* s);
char* cfstring_get_cstring(CFStringRef text_ref);
const char* get_name_for_pid(uint64_t pid);
const char* read_file(char* path);
bool vfork_exec(char *command, struct env_vars* env_vars);
