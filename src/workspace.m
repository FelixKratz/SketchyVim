#include "workspace.h"
#include "buffer.h"
#include "event_tap.h"

void workspace_begin(void **context) {
    workspace_context *ws_context = [workspace_context alloc];
    *context = ws_context;

    [ws_context init];
}

@implementation workspace_context
- (id)init {
    if ((self = [super init])) {
        [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                selector:@selector(appSwitched:)
                name:NSWorkspaceDidActivateApplicationNotification
                object:nil];
    }

    return self;
}

- (void)dealloc {
    [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:self];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];
    [super dealloc];
}

- (void)appSwitched:(NSNotification *)notification {
    char* name = NULL;
    char* bundle_id = NULL;
    if (notification && notification.userInfo) {
      NSRunningApplication* app = [notification.userInfo objectForKey:NSWorkspaceApplicationKey];
      if (app) {
        name = string_copy((char*)[[app localizedName] UTF8String]);
        bundle_id = string_copy((char*)[[app bundleIdentifier] UTF8String]);
      }
    }

    g_event_tap.front_app_ignored = event_tap_check_blacklist(&g_event_tap,
                                                              name,
                                                              bundle_id);
    if (name) free(name);
    if (bundle_id) free(bundle_id);
}

@end
