#import <Cocoa/Cocoa.h>

#import "IPCurveStorage.h"
#import "IPCurveView.h"

@interface IPSourceController : NSObject <NSTableViewDelegate>
{
    IBOutlet NSTableView * curveSetChooser;

    IBOutlet IPCurveView * controlPointView;
    IBOutlet IPCurveStorage * curveStorage;
}

- (int)numberOfRowsInTableView:(NSTableView *)tableView;

- (id)tableView:(NSTableView *)tableView
    objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row;

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification;

@end
