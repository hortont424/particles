#import <Cocoa/Cocoa.h>

//#import "IPControlPointView.h"

@interface IPControlPointController : NSObject
{
    IBOutlet NSSegmentedControl * controlPointType;
    IBOutlet NSSegmentedControl * drawControlPoints;
    
    //IBOutlet IPControlPointView * controlPointView;
}

- (IBAction)setControlPointType:(id)sender;
- (IBAction)setDrawControlPoints:(id)sender;

@end
