#import "IPControlPointController.h"

@implementation IPControlPointController

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
