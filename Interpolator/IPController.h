#import <Cocoa/Cocoa.h>

#import "IPCurveView.h"
#import "IPCurveStorage.h"

@interface IPController : NSObject
{
    IBOutlet NSSegmentedControl * controlPointTypeButtons;
    IBOutlet NSButton * drawControlPointsButton;
    IBOutlet NSOutlineView * curveSetChooser;

    IBOutlet NSMenu * addMenu, * removeMenu;

    IBOutlet IPCurveView * controlPointView;
	IBOutlet IPCurveStorage * curveStorage;
}

- (IBAction)setControlPointType:(id)sender;
- (IBAction)setDrawControlPoints:(id)sender;

- (IBAction)addCurveSet:(id)sender;
- (IBAction)removeCurveSet:(id)sender;

- (IBAction)addCurve:(id)sender;
- (IBAction)removeCurve:(id)sender;

- (void)updateCurveSets;

@end
