#include "Carbon/Carbon.h"
#include "Cocoa/Cocoa.h"
#include "event_tap.h"
#include "ax.h"
#include "workspace.h"

void* g_workspace;

int main (int argc, char *argv[]) {
  NSApplicationLoad();

  ax_begin(&g_ax);
  event_tap_begin(&g_event_tap);
  workspace_begin(&g_workspace);

  CFRunLoopRun();
  return 0;
}
