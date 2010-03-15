#import <Cocoa/Cocoa.h>

#import "IPCurveView.h"
#import "IPCurveStorage.h"

@interface IPController : NSObject
{
    IBOutlet NSSegmentedControl * controlPointTypeButtons;
    IBOutlet NSSegmentedControl * drawControlPointsButton;
    IBOutlet NSTableView * curveSetChooser;

    IBOutlet IPCurveView * controlPointView;
	IBOutlet IPCurveStorage * curveStorage;
}

- (IBAction)setControlPointType:(id)sender;
- (IBAction)setDrawControlPoints:(id)sender;

- (IBAction)addCurveSet:(id)sender;
- (IBAction)removeCurveSet:(id)sender;

- (void)updateCurveSets;

@end
