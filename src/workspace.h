#pragma once
#include <Cocoa/Cocoa.h>

char* g_front_app;

extern char* string_copy(char* s);

@interface workspace_context : NSObject {
}
- (id)init;
@end

void workspace_begin(void **context);
