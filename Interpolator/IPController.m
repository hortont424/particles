#import "IPController.h"
#import "IPCurveSet.h"

@implementation IPController

- (void)awakeFromNib
{
    [curveStorage load];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
    [curveStorage save];
}

- (IBAction)setControlPointType:(id)sender
{
    [controlPointView setSelectedControlPointType:
        [controlPointTypeButtons selectedSegment]];
}

- (IBAction)setDrawControlPoints:(id)sender
{
    [controlPointView setDrawControlPoints:![sender state]];
}

- (IBAction)addCurveSet:(id)sender
{
    IPCurveSet * newSet = [curveStorage addCurveSet];
    [self updateCurveSets];
}

- (IBAction)removeCurveSet:(id)sender
{
    NSLog(@"remove curve set");
}

- (IBAction)addCurve:(id)sender
{
    // Find selected curve set (if a curve is selected, take its parent)
    id item = [curveSetChooser itemAtRow:[curveSetChooser selectedRow]];

    if([item isMemberOfClass:[IPCurve class]])
        item = [curveSetChooser parentForItem:item];

    IPControlPoint * pt;
    IPCurve * curve = [[IPCurve alloc] init];

    pt = [[IPControlPoint alloc] init];
    [pt setPoint:NSMakePoint(20, 20)];
    [pt setType:CU_CONTROL_POINT_CORNER];
    [pt setControlPoint:0 toPoint:NSMakePoint(-2, -12)];
    [pt setControlPoint:1 toPoint:NSMakePoint(15, 15)];
    [curve addControlPoint:pt];

    pt = [[IPControlPoint alloc] init];
    [pt setPoint:NSMakePoint(120, 120)];
    [pt setType:CU_CONTROL_POINT_CORNER];
    [pt setControlPoint:0 toPoint:NSMakePoint(-2, -12)];
    [pt setControlPoint:1 toPoint:NSMakePoint(15, 15)];
    [curve addControlPoint:pt];

    [item addCurve:curve];
    [self updateCurveSets];
    [controlPointView updateCurves];
}

- (IBAction)removeCurve:(id)sender
{

}

- (void)updateCurveSets
{
    [curveSetChooser reloadData];
}

- (void)enableControlPointTypeButtons
{
    [controlPointTypeButtons setEnabled:YES];
}

- (void)disableControlPointTypeButtons
{
    [controlPointTypeButtons setEnabled:NO];
}

- (void)updateControlPointType:(int)type
{
    [controlPointTypeButtons setSelectedSegment:type];
}

@end
