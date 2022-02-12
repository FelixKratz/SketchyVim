#include "helpers.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
bool vfork_exec(char *command, struct env_vars* env_vars) {
    int pid = vfork();
    if (pid == -1) return false;
    if (pid !=  0) return true;

    alarm(FORK_TIMEOUT);

    if (env_vars) {
      for (int i = 0; i < env_vars->count; i++) {
        setenv(env_vars->vars[i]->key, env_vars->vars[i]->value, 1);
      }
    }

    char *exec[] = { "/usr/bin/env", "sh", "-c", command, NULL};
    exit(execvp(exec[0], exec));
}
#pragma clang diagnostic pop

char* cfstring_get_cstring(CFStringRef text_ref) {
  CFIndex length = CFStringGetLength(text_ref);
  uint32_t size = CFStringGetMaximumSizeForEncoding(length,
                                                    kCFStringEncodingUTF8);
  char* buf = malloc(sizeof(char)*(size + 1)); 
  CFStringGetCString(text_ref, buf, size + 1, kCFStringEncodingUTF8);
  return buf;
}

char* string_copy(char* s) {
  int length = strlen(s);
  char* result = malloc(length + 1);
  if (!result) return NULL;

  memcpy(result, s, length);
  result[length] = '\0';
  return result;
}

const char* get_name_for_pid(uint64_t pid) {
  return [[[NSRunningApplication runningApplicationWithProcessIdentifier:pid] localizedName] UTF8String];
}

const char* read_file(char* path) {
  struct stat buffer;

  if (stat(path, &buffer) != 0 || buffer.st_mode & S_IFDIR)
    return NULL;

  int fd = open(path, O_RDONLY);
  int len = lseek(fd, 0, SEEK_END);
  char* file = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);

  return string_copy(file);
}

