#import <Cocoa/Cocoa.h>

@interface IPSourceController : NSObject
{

}

- (int)numberOfRowsInTableView:(NSTableView *)tableView;

- (id)tableView:(NSTableView *)tableView
    objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row;

@end
