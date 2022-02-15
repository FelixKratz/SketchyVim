
#include "workspace.h"

void workspace_begin(void **context) {
    workspace_context *ws_context = [workspace_context alloc];
    *context = ws_context;
    g_front_app = NULL;

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
    if (notification && notification.userInfo) {
      NSRunningApplication* app = [notification.userInfo objectForKey:NSWorkspaceApplicationKey];
      if (app) name = string_copy((char*)[[app localizedName] UTF8String]);
    }

    if (g_front_app) free(g_front_app);
    g_front_app = name;
}

@end
