#ifndef ENGINE_COCOA_DIALOGS_H
#define ENGINE_COCOA_DIALOGS_H

#ifdef __cplusplus
extern "C" void cocoa_error_window(const char *title, const char *description);
#else
void cocoa_error_window(const char *title, const char *description);
#endif

#endif // !ENGINE_COCOA_DIALOGS_H
