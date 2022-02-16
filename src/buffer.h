#pragma once
#include <stdint.h>
#include <wchar.h>
#include <CoreText/CoreText.h>
#include "libvim/libvim.h"
#include "line.h"
#include "env_vars.h"

#define BUFFER_CLEAR "%d"
#define NORMAL_MODE  "<esc>"
#define INSERT_MODE  "i"

#define VISUAL_LINE  0x56

extern const char* read_file(char* path);
extern bool vfork_exec(char *command, struct env_vars* env_vars);
extern char* string_copy(char* s);

struct cursor {
  uint32_t mode;
  long position;
  long selection;
};

struct buffer {
  buf_T* vbuf;
  char_u* raw;

  bool did_change;
  struct line command_line;
  char cmd_line_mode;

  struct line** lines;
  uint32_t line_count;

  struct cursor cursor;
};

void buffer_begin(struct buffer* buffer);
void buffer_input(struct buffer* buffer, UniChar key, UniCharCount count);
void buffer_clear(struct buffer* buffer);
void buffer_revsync_text(struct buffer* buffer);
void buffer_revsync_cursor(struct buffer* buffer);
void buffer_call_script(struct buffer* buffer, bool supported);

struct line* line_create();
void line_destroy(struct line* line);
