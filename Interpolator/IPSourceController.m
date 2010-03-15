#import "IPSourceController.h"

@implementation IPSourceController

- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
    return [[curveStorage curveSets] count];
}

- (id)tableView:(NSTableView *)tableView
    objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row
{
    return [[[curveStorage curveSets] objectAtIndex:row] name];
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
    [controlPointView setCurvesIndex:[curveSetChooser selectedRow]];
    [controlPointView updateCurves];
}

@end
