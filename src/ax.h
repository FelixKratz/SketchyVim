#pragma once
#include <Carbon/Carbon.h>
#include "buffer.h"

#define ROLE_TEXT   1
#define ROLE_TABLE  1 << 1
#define ROLE_SCROLL 1 << 2

#define FLAG_SHIFT   1 << 17
#define FLAG_COMMAND 1 << 20

#define ENTER  0x0D
#define ESCAPE 0x1B
#define LEFT   0x1C
#define RIGHT  0x1D
#define UP     0x1E
#define DOWN   0x1F

// HACK: These are hardcoded for my workflow for now
// TODO: Infere movement keys from mappings
#define J  0x6A
#define K  0x6B
#define L  0x6C
#define OE 0xF6
//////////////////////

#define kAXWebAreaRole CFSTR("AXWebArea")

extern char* cfstring_get_cstring(CFStringRef text_ref);

struct ax {
  bool is_privileged;
  bool is_supported;

  uint32_t role;
  CFTypeRef selected_element;
  AXUIElementRef system_element;

  struct buffer buffer;
};

struct ax g_ax;
void ax_begin(struct ax* ax);
void ax_clear(struct ax* ax);

CGEventRef ax_process_event(struct ax* ax, CGEventRef event);
