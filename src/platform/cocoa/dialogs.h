#ifndef PLATFORM_COCOA_DIALOGS_H
#define PLATFORM_COCOA_DIALOGS_H

#ifdef __cplusplus
extern "C" void cocoa_error_window(const char *title, const char *description);
#else
void cocoa_error_window(const char *title, const char *description);
#endif

#endif // !PLATFORM_COCOA_DIALOGS_H
