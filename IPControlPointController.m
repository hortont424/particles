#import "IPControlPointController.h"

@implementation IPControlPointController

- (IBAction)setControlPointType:(id)sender
{
    //[controlPointType setEnabled:YES];
	NSLog(@"%p", controlPointType);
    NSLog(@"d: %@", [controlPointType labelForSegment:0]);
}

- (IBAction)setDrawControlPoints:(id)sender
{
    /*[controlPointView setDrawControlPoints:
        [drawControlPoints isSelectedForSegment:0]];
    [drawControlPoints setLabel:@"Import" forSegment:0];
    NSLog(@"%@", [drawControlPoints labelForSegment:0]);
    NSLog(@"%@", [controlPointType labelForSegment:0]);*/
}

@end
