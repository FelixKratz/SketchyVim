#include "window_detector.h"
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Forward declaration of helper function
extern char *cfstring_get_cstring(CFStringRef text_ref);
extern char *string_copy(char *s);

static pthread_t detector_thread;
static bool detector_running = false;
static window_detector_callback user_callback = NULL;
static char **watched_apps = NULL;
static int watched_apps_count = 0;
static bool *last_visibility_states = NULL;

// Check if a specific application has any visible windows
bool window_detector_is_app_visible(const char *app_name)
{
    CFArrayRef window_list = CGWindowListCopyWindowInfo(
        kCGWindowListOptionOnScreenOnly,
        kCGNullWindowID);

    if (!window_list)
    {
        return false;
    }

    CFIndex window_count = CFArrayGetCount(window_list);
    bool found = false;

    for (CFIndex i = 0; i < window_count; i++)
    {
        CFDictionaryRef window_info = (CFDictionaryRef)CFArrayGetValueAtIndex(window_list, i);
        if (!window_info)
            continue;

        CFStringRef owner_name = (CFStringRef)CFDictionaryGetValue(window_info, kCGWindowOwnerName);
        if (owner_name)
        {
            char *owner_cstring = cfstring_get_cstring(owner_name);
            if (owner_cstring && strcmp(owner_cstring, app_name) == 0)
            {
                found = true;
                free(owner_cstring);
                break;
            }
            if (owner_cstring)
                free(owner_cstring);
        }
    }

    CFRelease(window_list);
    return found;
}

// Load watched apps from blacklist
static void load_watched_apps(void)
{
    char *home = getenv("HOME");
    char buf[512];
    snprintf(buf, sizeof(buf), "%s/%s", home, ".config/svim/blacklist");

    FILE *file = fopen(buf, "r");
    if (!file)
        return;

    char line[255];
    while (fgets(line, 255, file))
    {
        uint32_t len = strlen(line);
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';

        // Skip empty lines
        if (len <= 1)
            continue;

        watched_apps = realloc(watched_apps, sizeof(char *) * (watched_apps_count + 1));
        last_visibility_states = realloc(last_visibility_states, sizeof(bool) * (watched_apps_count + 1));

        watched_apps[watched_apps_count] = string_copy(line);
        last_visibility_states[watched_apps_count] = false;
        watched_apps_count++;
    }
    fclose(file);
}

// Polling thread function
static void *detector_thread_func(void *arg)
{
    (void)arg; // Unused parameter

    while (detector_running)
    {
        for (int i = 0; i < watched_apps_count; i++)
        {
            bool current_visible = window_detector_is_app_visible(watched_apps[i]);

            // Check for state change
            if (current_visible != last_visibility_states[i])
            {
                if (user_callback)
                {
                    user_callback(watched_apps[i], current_visible);
                }
                last_visibility_states[i] = current_visible;
            }
        }

        // Sleep for 0.5 seconds
        usleep(500000); // 500ms = 0.5s
    }

    return NULL;
}

void window_detector_set_callback(window_detector_callback callback)
{
    user_callback = callback;
}

void window_detector_begin(void)
{
    if (detector_running)
        return;

    load_watched_apps();

    if (watched_apps_count == 0)
    {
        return; // No apps to watch
    }

    detector_running = true;

    if (pthread_create(&detector_thread, NULL, detector_thread_func, NULL) != 0)
    {
        detector_running = false;
        return;
    }
}

void window_detector_end(void)
{
    if (!detector_running)
        return;

    detector_running = false;
    pthread_join(detector_thread, NULL);

    // Clean up
    for (int i = 0; i < watched_apps_count; i++)
    {
        if (watched_apps[i])
        {
            free(watched_apps[i]);
        }
    }

    if (watched_apps)
    {
        free(watched_apps);
        watched_apps = NULL;
    }

    if (last_visibility_states)
    {
        free(last_visibility_states);
        last_visibility_states = NULL;
    }

    watched_apps_count = 0;
    user_callback = NULL;
}