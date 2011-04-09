#import <platform/cocoa/dialogs.h>
#import <Cocoa/Cocoa.h>

void cocoa_error_window(const char *title, const char *description) {
  NSAlert *alert = [[NSAlert alloc] init];
  NSString *errorTitle = [[NSString alloc] initWithCString:title];
  NSString *errorDescription = [[NSString alloc] initWithCString:description];
  
  [alert setMessageText:errorTitle];
  
  /*if (reportButton) {
    [alert addButtonWithTitle:@"Quit"];
    [alert addButtonWithTitle:@"Report..."];
    [alert setAlertStyle:NSCriticalAlertStyle];
  }
  else {*/
    [alert addButtonWithTitle:@"Dismiss"];
    [alert setAlertStyle:NSWarningAlertStyle];		
    //}
  
  [alert setInformativeText:errorDescription];
  
  if ([alert runModal] == NSAlertSecondButtonReturn) {
    NSAlert * alert2 = [[NSAlert alloc] init];
    [alert2 setMessageText:@"Not fixed yet!"];
    [alert2 setInformativeText:@"I would appreciate if you sent your log manually to ptr.bckmn@gmail.com, a description of what went wrong and your machine's specifications."];
    [alert2 setAlertStyle:NSWarningAlertStyle];
    [alert2 runModal];
    [alert2 release];
  }
  
  [alert release];
}

