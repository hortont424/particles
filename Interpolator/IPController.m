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
    //[controlPointType setEnabled:YES];
}

- (IBAction)setDrawControlPoints:(id)sender
{
    [controlPointView setDrawControlPoints:
        [drawControlPointsButton isSelectedForSegment:0]];
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

}

- (IBAction)removeCurve:(id)sender
{

}

- (void)updateCurveSets
{
    [curveSetChooser reloadData];
}

@end
