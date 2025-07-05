#include "workspace.h"
#include "buffer.h"
#include "event_tap.h"

void workspace_begin(void **context) {
    workspace_context *ws_context = [workspace_context alloc];
    *context = ws_context;

    [ws_context init];
    
    // Start the window detector for apps like Raycast
    window_detector_begin();
}

@implementation workspace_context
- (id)init {
    if ((self = [super init])) {
        [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                selector:@selector(appSwitched:)
                name:NSWorkspaceDidActivateApplicationNotification
                object:nil];
        
        // Set up window detector callback
        window_detector_set_callback(window_detector_app_visibility_changed);
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
    pid_t pid = 0;
    if (notification && notification.userInfo) {
      NSRunningApplication* app = [notification.userInfo objectForKey:NSWorkspaceApplicationKey];
      if (app) {
        name = (char*)[[app localizedName] UTF8String];
        bundle_id = (char*)[[app bundleIdentifier] UTF8String];
        pid = app.processIdentifier;
      }
    }

    g_event_tap.front_app_ignored = event_tap_check_blacklist(&g_event_tap,
                                                              name,
                                                              bundle_id    );
    ax_front_app_changed(&g_ax, pid);
}

// Callback for window detector when app visibility changes
static void window_detector_app_visibility_changed(const char* app_name, bool is_visible) {
    if (is_visible) {
        printf("✅ %s appeared\n", app_name);
        g_event_tap.front_app_ignored = true;
    } else {
        printf("⚠️ %s disappeared\n", app_name);
        // Check if any other blacklisted app is still visible
        g_event_tap.front_app_ignored = false;
        // We'll let the next app switch notification handle the final state
    }
}

@end

void workspace_end(void **context) {
    if (context && *context) {
        workspace_context *ws_context = (workspace_context *)*context;
        [ws_context dealloc];
        *context = NULL;
    }
    
    // Stop the window detector
    window_detector_end();
}
