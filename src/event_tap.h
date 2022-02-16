#pragma once
#include <stdbool.h>
#include <Carbon/Carbon.h>
#include <stdint.h>
#include <wchar.h>
#include "ax.h"
#include "buffer.h"

extern const char* get_name_for_pid(uint64_t pid);
extern char* string_copy(char* s);
extern bool g_front_app_ignored;

struct event_tap {
  uint32_t blacklist_count;
  char** blacklist;
  CFMachPortRef handle;
  CFRunLoopSourceRef runloop_source;
  CGEventMask mask;
};

struct event_tap g_event_tap;
bool event_tap_enabled(struct event_tap *event_tap);
bool event_tap_begin(struct event_tap *event_tap);
void event_tap_end(struct event_tap *event_tap);
bool event_tap_app_blacklisted(struct event_tap* event_tap, char* app);
