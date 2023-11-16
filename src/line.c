#include "line.h"
#include <stdio.h>
#include <string.h>

static inline uint32_t unicode_character_count(char* text) {
  char* read = text;
  uint32_t counter = 0;
  while (*read) { counter += ((*read++ & 0xC0) != 0x80); }
  return counter;
}

static inline uint32_t char_count_for_unicode_char_count(char* text, uint32_t uni_count) {
  char* read = text;
  uint32_t uni_counter = 0;
  uint32_t byte_counter = 0;
  while (*read) {
    uni_counter += ((*read++ & 0xC0) != 0x80);
    byte_counter++;
    if (uni_counter >= uni_count) break;
  }

  return byte_counter;
}

struct line* line_create() {
  struct line* line = malloc(sizeof(struct line));
  memset(line, 0, sizeof(struct line));
  return line;
}

void line_set_text(struct line* line, char* text) {
  if (!text) return;

  line->length = unicode_character_count(text);

  uint32_t len = strlen(text);
  line->raw = realloc(line->raw, sizeof(char) * (len + 1));
  memcpy(line->raw, text, sizeof(char) * (len + 1));
  line->raw_length = len;
}

void line_clear(struct line* line) {
  if (line->raw) free(line->raw);
  line->raw = NULL;
  line->cursor_offset = 0;
  line->length = 0;
  line->raw_length = 0;
}

void line_destroy(struct line* line) {
  line_clear(line);
  free(line);
}

uint32_t line_get_position_from_raw_position(struct line* line,
                                             uint32_t raw_pos) {
  if (line->raw_length >= raw_pos) {
    char raw[raw_pos + 1];

    memcpy(raw, line->raw, sizeof(char) * raw_pos);
    raw[raw_pos] = '\0';

    return unicode_character_count(raw);
  }
  return 0;
}

uint32_t line_get_raw_position_from_position(struct line* line, uint32_t pos) {
  return (line->length >= pos)
         ? char_count_for_unicode_char_count(line->raw, pos)
         : 0;
}

