#import "IPSourceController.h"

@implementation IPSourceController

- (void)outlineViewSelectionDidChange:(NSNotification *)aNotification
{
    id item = [curveSetChooser itemAtRow:[curveSetChooser selectedRow]];

    if([item isMemberOfClass:[IPCurve class]])
        item = [curveSetChooser parentForItem:item];

    [controlPointView setCurvesIndex:[curveSetChooser rowForItem:item]];
}

- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object
    forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
    IPCurveSet * set = item;
    [set setName:(NSString *)object];
}

- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index
    ofItem:(id)item
{
    if(item == nil)
    {
        return [[curveStorage curveSets] objectAtIndex:index];
    }
    else
    {
        if([item isMemberOfClass:[IPCurveSet class]])
            return [[item curves] objectAtIndex:index];
    }

    return nil;
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView
    numberOfChildrenOfItem:(id)item
{
    if(item == nil)
    {
        return [[curveStorage curveSets] count];
    }
    else
    {
        if([item isMemberOfClass:[IPCurveSet class]])
            return [[item curves] count];
    }

    return 0;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
    return [item isMemberOfClass:[IPCurveSet class]];
}

- (id)outlineView:(NSOutlineView *)outlineView
    objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
    return [item name];
}

@end
