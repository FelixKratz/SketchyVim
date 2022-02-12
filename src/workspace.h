#include <Cocoa/Cocoa.h>
#pragma once

char* g_front_app;

extern char* string_copy(char* s);

@interface workspace_context : NSObject {
}
- (id)init;
@end

void workspace_begin(void **context);
