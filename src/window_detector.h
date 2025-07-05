#pragma once
#include <stdbool.h>
#include <CoreGraphics/CoreGraphics.h>

// Function to check if a specific application has any visible windows
bool window_detector_is_app_visible(const char *app_name);

// Function to start the window detection polling
void window_detector_begin(void);

// Function to stop the window detection polling
void window_detector_end(void);

// Callback function type for when app visibility changes
typedef void (*window_detector_callback)(const char *app_name, bool is_visible);

// Set the callback function
void window_detector_set_callback(window_detector_callback callback);