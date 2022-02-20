#include "event_tap.h"

bool event_tap_check_blacklist(struct event_tap* event_tap,
                               char* app, char* bundle_id  ) {
  if (!app || !bundle_id) return true;
  for (int i = 0; i < event_tap->blacklist_count; i++)
    if (strcmp(event_tap->blacklist[i], app) == 0 
        || strcmp(event_tap->blacklist[i], bundle_id) == 0) {
      return true;
    }

  return false;
}

static CGEventRef key_handler(CGEventTapProxy proxy, CGEventType type,
                              CGEventRef event, void* reference) {
  switch (type) {
    case kCGEventTapDisabledByTimeout:
      printf("Timeout\n");
    case kCGEventTapDisabledByUserInput: {
      printf("restarting event-tap\n");
      CGEventTapEnable(((struct event_tap*) reference)->handle, true);
    } break;
    case kCGEventKeyDown: {
      if (((struct event_tap*) reference)->front_app_ignored) {
        if (g_ax.selected_element && g_ax.role) {
          ax_clear(&g_ax);
        }
        return event;
      }

      return ax_process_event(&g_ax, event);
    } break;
  }
  return event;
}

bool event_tap_enabled(struct event_tap* event_tap) {
  bool result = (event_tap->handle && CGEventTapIsEnabled(event_tap->handle));
  return result;
}

void event_tap_load_blacklist(struct event_tap* event_tap) {
  event_tap->blacklist = NULL;
  event_tap->blacklist_count = 0;
  event_tap->front_app_ignored = true;

  char* home = getenv("HOME");
  char buf[512];
  snprintf(buf, sizeof(buf), "%s/%s", home, ".config/svim/blacklist");

  FILE *file = fopen(buf, "r");

  if (!file) return;

  char line[255];
  while (fgets(line, 255, file)) {
    uint32_t len = strlen(line);
    if (line[len - 1] == '\n') line[len - 1] = '\0';
    event_tap->blacklist = realloc(event_tap->blacklist,
                                sizeof(char**) * ++event_tap->blacklist_count);

    event_tap->blacklist[event_tap->blacklist_count - 1] = string_copy(line);
  }
  fclose(file);
}

bool event_tap_begin(struct event_tap* event_tap) {
  event_tap_load_blacklist(event_tap);

  event_tap->mask = 1 << kCGEventKeyDown;
  event_tap->handle = CGEventTapCreate(kCGAnnotatedSessionEventTap,
                                       kCGHeadInsertEventTap,
                                       kCGEventTapOptionDefault,
                                       event_tap->mask,
                                       &key_handler,
                                       event_tap);

  bool result = event_tap_enabled(event_tap);
  if (result) {
    event_tap->runloop_source = CFMachPortCreateRunLoopSource(
                                                           kCFAllocatorDefault,
                                                           event_tap->handle,
                                                           0);
    CFRunLoopAddSource(CFRunLoopGetMain(),
                       event_tap->runloop_source,
                       kCFRunLoopCommonModes);
  }

  return result;
}

void event_tap_end(struct event_tap* event_tap) {
  if (event_tap_enabled(event_tap)) {
    CGEventTapEnable(event_tap->handle, false);
    CFMachPortInvalidate(event_tap->handle);
    CFRunLoopRemoveSource(CFRunLoopGetMain(),
                          event_tap->runloop_source,
                          kCFRunLoopCommonModes);
    CFRelease(event_tap->runloop_source);
    CFRelease(event_tap->handle);
    event_tap->handle = NULL;

    for (int i = 0; i < event_tap->blacklist_count; i++)
      if (event_tap->blacklist[i]) free(event_tap->blacklist[i]);

    if (event_tap->blacklist) free(event_tap->blacklist);
  }
}
