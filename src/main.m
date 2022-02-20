#include "Carbon/Carbon.h"
#include "Cocoa/Cocoa.h"
#include "event_tap.h"
#include "ax.h"
#include "workspace.h"

void* g_workspace;

static void acquire_lockfile(void) {
  int handle = open("/tmp/svim.lock", O_CREAT | O_WRONLY, 0600);
  if (handle == -1) printf("Error: Could not create lock-file.\n"), exit(1);

  struct flock lockfd = {
    .l_start  = 0,
    .l_len    = 0,
    .l_pid    = getpid(),
    .l_type   = F_WRLCK,
    .l_whence = SEEK_SET
  };

  if (fcntl(handle, F_SETLK, &lockfd) == -1)
    printf("Error: Could not acquire lock-file.\n"), exit(1);
}

int main (int argc, char *argv[]) {
  NSApplicationLoad();

  acquire_lockfile();
  ax_begin(&g_ax);
  event_tap_begin(&g_event_tap);
  workspace_begin(&g_workspace);

  CFRunLoopRun();
  return 0;
}
