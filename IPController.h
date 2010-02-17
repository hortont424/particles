#import <Cocoa/Cocoa.h>

#import "IPCurveView.h"
#import "IPCurveStorage.h"

@interface IPController : NSObject
{
    IBOutlet NSSegmentedControl * controlPointType;
    IBOutlet NSSegmentedControl * drawControlPoints;
    
    IBOutlet IPCurveView * controlPointView;
	
	IBOutlet IPCurveStorage * curveStorage;
}

- (IBAction)changeControlPointType:(id)sender;
- (IBAction)changeDrawControlPoints:(id)sender;

@end
