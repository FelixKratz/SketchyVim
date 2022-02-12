#pragma once
#include <wchar.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct line {
  uint32_t length;
  uint32_t raw_length;
  wchar_t* text;
  char* raw;
};

struct line* line_create();
void line_clear(struct line* line);
void line_destroy(struct line* line);
void line_set_text(struct line* line, char* text);
uint32_t line_get_position_from_raw_position(struct line* line, uint32_t raw_pos);
uint32_t line_get_raw_position_from_position(struct line* line, uint32_t pos);
