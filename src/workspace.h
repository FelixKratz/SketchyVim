#pragma once
#include <Cocoa/Cocoa.h>
#include "event_tap.h"
#include "window_detector.h"

bool g_front_app_ignored;

extern char *string_copy(char *s);

@interface workspace_context : NSObject
{
}
- (id)init;
@end

void workspace_begin(void **context);
void workspace_end(void **context);
