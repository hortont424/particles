#import <Cocoa/Cocoa.h>

#import "IPCurveView.h"
#import "IPCurveStorage.h"

@interface IPController : NSObject
{
    IBOutlet NSSegmentedControl * controlPointTypeButtons;
    IBOutlet NSOutlineView * curveSetChooser;

    IBOutlet IPCurveView * controlPointView;
	IBOutlet IPCurveStorage * curveStorage;
}

- (IBAction)setControlPointType:(id)sender;
- (IBAction)setDrawControlPoints:(id)sender;

- (IBAction)addCurveSet:(id)sender;
- (IBAction)removeCurveSet:(id)sender;

- (IBAction)addCurve:(id)sender;
- (IBAction)removeCurve:(id)sender;

- (void)enableControlPointTypeButtons;
- (void)disableControlPointTypeButtons;
- (void)updateControlPointType:(int)type;

- (void)updateCurveSets;

@end
