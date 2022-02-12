#pragma once
#include <stdlib.h>
#include <string.h>

struct key_value_pair {
  char* key;
  char* value;
};

struct env_vars {
  uint32_t count;
  struct key_value_pair** vars;
};

void env_vars_init(struct env_vars* env_vars);
void env_vars_unset(struct env_vars* env_vars, char* key);
void env_vars_set(struct env_vars* env_vars, char* key, char* value);
char* env_vars_get_value_for_key(struct env_vars* env_vars, char* key);
void env_vars_destroy(struct env_vars* env_vars);
