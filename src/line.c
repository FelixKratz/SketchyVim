#include "line.h"
#include <string.h>

struct line* line_create() {
  struct line* line = malloc(sizeof(struct line));
  memset(line, 0, sizeof(struct line));
  return line;
}

void line_set_text(struct line* line, char* text) {
  if (!text) return;

  uint32_t len = strlen(text);
  line->text = realloc(line->text, sizeof(wchar_t) * (len + 1));
  swprintf(line->text, len + 1, L"%s", text);
  line->text[len] = '\0';
  line->length = wcslen(line->text);

  line->raw = realloc(line->raw, sizeof(char) * (len + 1));
  memcpy(line->raw, text, sizeof(char) * (len + 1));
  line->raw_length = len;
}

void line_clear(struct line* line) {
  if (line->text) free(line->text);
  if (line->raw) free(line->raw);
  line->raw = NULL;
  line->text = NULL;
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
    wchar_t wide[raw_pos + 1];

    memcpy(raw, line->raw, sizeof(char) * raw_pos);
    raw[raw_pos] = '\0';

    swprintf(wide, raw_pos + 1, L"%s", raw);
    return wcslen(wide);
  }
  return 0;
}

uint32_t line_get_raw_position_from_position(struct line* line, uint32_t pos) {
  if (line->length >= pos) {
    wchar_t wide[pos + 1];
    char raw[2*pos + 1];

    memcpy(wide, line->text, sizeof(wchar_t) * pos);
    wide[pos] = '\0';

    snprintf(raw, 2*pos + 1, "%S", wide);
    return strlen(raw);
  }
  return 0;
}

