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

- (IBAction)changeControlPointType:(id)sender
{
    //[controlPointType setEnabled:YES];
}

- (IBAction)changeDrawControlPoints:(id)sender
{
    [controlPointView setDrawControlPoints:
        [drawControlPoints isSelectedForSegment:0]];
}

@end
