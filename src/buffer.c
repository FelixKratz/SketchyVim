#include "buffer.h"
#include "env_vars.h"
#include "line.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void buffer_loadrc(struct buffer* buffer) {
  char* home = getenv("HOME");
  char buf[512];
  snprintf(buf, sizeof(buf), "%s/%s", home, ".config/svim/svimrc");

  char_u* file = (char_u*) read_file(buf);
  if (file) {
    vimExecute(file);
    free(file);
  } 
  vimExecute("set nocindent nosmartindent noautoindent");
}

void buffer_begin(struct buffer* buffer) {
  vimInit(0, NULL);
  buffer->vbuf = vimBufferNew(0);
  vimBufferSetCurrent(buffer->vbuf);
  buffer_loadrc(buffer);
}

void buffer_update_raw_text(struct buffer* buffer) {
  uint32_t lines = vimBufferGetLineCount(buffer->vbuf);

  char_u* raw = NULL;
  uint32_t len = 0;
  for (int i = 1; i <= lines; i++) {
    char_u* line = vimBufferGetLine(buffer->vbuf, i);
    uint32_t line_len = strlen(line);

    len += line_len + 1;

    raw = realloc(raw, sizeof(char_u) * len);
    if (i == 1) snprintf(raw, len, "%s", line);
    else snprintf(raw, len, "%s\n%s", raw, line);
  }
  raw[len - 1] = '\0';

  if (buffer->raw && raw && strcmp(buffer->raw, raw) == 0) {
    free(raw);
    buffer->did_change = false;
  }
  else {
    if (buffer->raw) free(buffer->raw);
    buffer->raw = raw;
    buffer->did_change = true;
  }
}

void buffer_sync_text(struct buffer* buffer) {
  buffer_update_raw_text(buffer);

  for (int i = 0; i < buffer->line_count; i++)
    line_destroy(buffer->lines[i]);

  uint32_t lines = vimBufferGetLineCount(buffer->vbuf);
  buffer->lines = realloc(buffer->lines, sizeof(struct line*) * lines);
  memset(buffer->lines, 0, sizeof(struct line*) * lines);
  buffer->line_count = lines;

  for (int i = 1; i <= lines; i++) {
    char_u* line = vimBufferGetLine(buffer->vbuf, i);
    buffer->lines[i - 1] = line_create();
    line_set_text(buffer->lines[i - 1], line);
  }
}

void buffer_sync_cursor(struct buffer* buffer) {
  pos_T cursor_pos = vimCursorGetPosition();

  uint32_t cursor_pos_cummulative = 0;
  for (int i = 0; i < cursor_pos.lnum - 1; i++) {
    cursor_pos_cummulative += buffer->lines[i]->length + 1;
  }

  uint32_t pos = line_get_position_from_raw_position(buffer->lines[cursor_pos.lnum - 1], cursor_pos.col);

  buffer->cursor.position = cursor_pos_cummulative + pos;

  if (buffer->cursor.mode & NORMAL) {
    buffer->cursor.selection = 1;
  }
  else if (buffer->cursor.mode & VISUAL) {
    pos_T start, end;
    vimVisualGetRange(&start, &end);
    bool inverted = false;
    if (start.lnum > end.lnum || (start.lnum == end.lnum
                                  && start.col > end.col)) {
      vimVisualGetRange(&end, &start);
      inverted = true;
    }


    uint32_t start_pos = line_get_position_from_raw_position(buffer->lines[start.lnum - 1], start.col);
    uint32_t end_pos = line_get_position_from_raw_position(buffer->lines[end.lnum - 1], end.col);
    
    uint32_t selection = 0;
    for (int i = start.lnum; i < end.lnum; i++) {
      selection += buffer->lines[i - 1]->length + 1;
    }

    int visual_mode = vimVisualGetType();

    if (visual_mode == 0x56)
      selection += end_pos;
    else
      selection += end_pos - start_pos;

    buffer->cursor.position -= inverted ? 0 : selection;
    buffer->cursor.selection = selection + 1;
  }
  else
    buffer->cursor.selection = 0;
}

bool buffer_sync_mode(struct buffer* buffer) {
  uint32_t mode = vimGetMode();
  if (mode & buffer->cursor.mode) return false;
  buffer->cursor.mode = mode;

  return true;
}

bool buffer_sync_cmdline(struct buffer* buffer) {
  char* old = buffer->command_line.raw ? string_copy(buffer->command_line.raw) : NULL;

  line_set_text(&buffer->command_line, vimCommandLineGetText());

  if (buffer->command_line.raw && old && strcmp(old, buffer->command_line.raw) == 0) {
    
    free(old);
    return false;
  }
  if (old) free(old);
  
  if (!buffer->command_line.raw) return false;

  return true;
}

// TODO: Hook the event when an item is deselected and call cleanup script
void buffer_sync(struct buffer* buffer) {
  bool did_change = buffer_sync_mode(buffer);

  buffer_sync_text(buffer);

  did_change |= buffer_sync_cmdline(buffer);
  buffer_sync_cursor(buffer);

  if (did_change) {
    char mode_str[3];
    if (buffer->cursor.mode & INSERT) {
      snprintf(mode_str, 2, "I");
    }
    else if (buffer->cursor.mode & NORMAL) {
      snprintf(mode_str, 2, "N");
    }
    else if (buffer->cursor.mode & VISUAL) {
      snprintf(mode_str, 2, "V");
    }
    else if (buffer->cursor.mode & CMDLINE) {
      snprintf(mode_str, 2, "C");
    }
    // TODO: Add pending command modes
    else
      snprintf(mode_str, 2, "");

    mode_str[2] = '\0';

    struct env_vars env_vars;
    env_vars_init(&env_vars);
    env_vars_set(&env_vars, string_copy("MODE"),
                            string_copy(mode_str));

    env_vars_set(&env_vars, string_copy("CMDLINE"),
                            string_copy((buffer->command_line.raw
                                         ? buffer->command_line.raw
                                         : "")));

    char* home = getenv("HOME");
    char buf[512];
    snprintf(buf, sizeof(buf), "%s/%s", home, ".config/svim/svim.sh");
    vfork_exec(buf, &env_vars);
    env_vars_destroy(&env_vars);
  }
}

void buffer_input(struct buffer* buffer, UniChar key, UniCharCount count) {
  if (key == 0x1B) {
    vimKey(NORMAL_MODE);
  }
  else {
    char_u* key_str = malloc(sizeof(char_u) * (2 * count + 1));
    memset(key_str, 0, (2 * count + 1));
    snprintf(key_str, 2 * count + 1, "%lc", key);

    vimInput(key_str);
    free(key_str);
  }

  buffer_sync(buffer);
}

void buffer_revsync_text(struct buffer* buffer) {
  vimExecute(BUFFER_CLEAR);
  vimKey(NORMAL_MODE);
  vimInput(INSERT_MODE);
  if (buffer->raw) vimInput(buffer->raw);
  if (buffer->cursor.mode & NORMAL) vimKey(NORMAL_MODE);
  buffer_sync_text(buffer);
  buffer_sync_cursor(buffer);
}

void buffer_revsync_cursor(struct buffer* buffer) {
  uint32_t character_count = 0;
  uint32_t line = 0;
  uint32_t line_character_count = 0;
  for (int i = 0; i < buffer->line_count; i++) {
    line_character_count = buffer->lines[i]->length;
    character_count += buffer->lines[i]->length + 1;
    line = i + 1;
    if (character_count > buffer->cursor.position) break;
  }

  pos_T pos = {0, 0, 0};
  pos.lnum = line;
  if (line > 0) {
    pos.col = line_get_raw_position_from_position(buffer->lines[line - 1],
                                                  buffer->cursor.position
                                                  + line_character_count
                                                  - character_count);
  }

  vimCursorSetPosition(pos);
  buffer_sync_cursor(buffer);
}

void buffer_clear(struct buffer* buffer) {
  if (buffer->raw) free(buffer->raw);
  for (int i = 0; i < buffer->line_count; i++)
    line_destroy(buffer->lines[i]);
  if (buffer->lines) free(buffer->lines);
  line_clear(&buffer->command_line);

  buffer->cursor = (struct cursor){0, 0, 0};
  buffer->did_change = false;
  buffer->line_count = 0;
  buffer->lines = NULL;
  buffer->raw = NULL;
  vimExecute(BUFFER_CLEAR);
  vimKey(NORMAL_MODE);
  vimInput(INSERT_MODE);
}
