#import <Cocoa/Cocoa.h>

#import "IPCurveView.h"

@interface IPController : NSObject
{
    IBOutlet NSSegmentedControl * controlPointType;
    IBOutlet NSSegmentedControl * drawControlPoints;
    
    IBOutlet IPCurveView * controlPointView;
}

- (IBAction)changeControlPointType:(id)sender;
- (IBAction)changeDrawControlPoints:(id)sender;

@end
