#import "IPSourceController.h"

@implementation IPSourceController

- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
    return 5;
}

- (id)tableView:(NSTableView *)tableView
    objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row
{
    return @"arst";
}

@end
