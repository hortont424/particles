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
