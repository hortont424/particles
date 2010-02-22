#import "IPController.h"

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
    NSLog(@"add curve set");
}

- (IBAction)removeCurveSet:(id)sender
{
    NSLog(@"remove curve set");
}

@end
