#include "line.h"
#include <stdint.h>
#include <string.h>
#include <wchar.h>

struct line* line_create() {
  struct line* line = malloc(sizeof(struct line));
  memset(line, 0, sizeof(struct line));
  return line;
}

bool line_compare_raw(struct line* line, char* raw) {
  if (line->raw && raw && strcmp(line->raw, raw) == 0)
    return true;
  return false;
}

void line_set_text(struct line* line, char* text) {
  if (!text) return;

  if (line_compare_raw(line, text)) {
    line->changed = false;
    return;
  }

  uint32_t len = strlen(text);
  line->text = realloc(line->text, sizeof(wchar_t) * (len + 1));
  swprintf(line->text, len + 1, L"%s", text);
  line->text[len] = '\0';
  line->length = wcslen(line->text);

  line->raw = realloc(line->raw, sizeof(char) * (len + 1));
  memcpy(line->raw, text, sizeof(char) * (len + 1));
  line->raw_length = len;
  line->changed = true;
}

void line_clear(struct line* line) {
  if (line->text) free(line->text);
  if (line->raw) free(line->raw);
  line->raw = NULL;
  line->text = NULL;
  line->length = 0;
  line->raw_length = 0;
}

void line_destroy(struct line* line) {
  line_clear(line);
  free(line);
}

uint32_t line_get_position_from_raw_position(struct line* line, uint32_t raw_pos) {
  if (line->raw_length >= raw_pos) {
    char* tmp = malloc(sizeof(char) * (raw_pos + 1));
    memcpy(tmp, line->raw, sizeof(char) * raw_pos);
    tmp[raw_pos] = '\0';

    wchar_t* tmp2 = malloc(sizeof(wchar_t) * (raw_pos + 1));
    memset(tmp2, 0, sizeof(wchar_t) * (raw_pos + 1));
    swprintf(tmp2, raw_pos + 1, L"%s", tmp);
    tmp2[raw_pos] = '\0';
    uint32_t len = wcslen(tmp2);
    free(tmp);
    free(tmp2);
    return len;
  }
  return 0;
}

uint32_t line_get_raw_position_from_position(struct line* line, uint32_t pos) {
  if (line->length >= pos) {
    wchar_t* tmp = malloc(sizeof(wchar_t) * (pos + 1));
    memcpy(tmp, line->text, sizeof(wchar_t) * pos);
    tmp[pos] = '\0';

    char* tmp2 = malloc(sizeof(char) * (2*pos + 1));
    memset(tmp2, 0, sizeof(char) * (2*pos + 1));
    snprintf(tmp2, 2*pos + 1, "%S", tmp);
    tmp2[2*pos] = '\0';
    uint32_t len = strlen(tmp2);
    free(tmp);
    free(tmp2);
    return len;
  }
  return 0;
}

