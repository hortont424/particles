#import <Cocoa/Cocoa.h>

#import "IPControlPointView.h"

@interface IPControlPointController : NSObject
{
    IBOutlet NSSegmentedControl * controlPointType;
    IBOutlet NSSegmentedControl * drawControlPoints;
    
    IBOutlet IPControlPointView * controlPointView;
}

- (IBAction)changeControlPointType:(id)sender;
- (IBAction)changeDrawControlPoints:(id)sender;

@end
