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

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject
    forTableColumn:(NSTableColumn *)aTableColumn row:(int)row
{
    IPCurveSet * set = [[curveStorage curveSets] objectAtIndex:row];
    [set setName:(NSString *)anObject];
}

@end
