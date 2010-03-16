#import <Cocoa/Cocoa.h>

#import "IPCurveStorage.h"
#import "IPCurveView.h"

@interface IPSourceController : NSObject <NSOutlineViewDelegate, NSOutlineViewDataSource>
{
    IBOutlet NSOutlineView * curveSetChooser;

    IBOutlet IPCurveView * controlPointView;
    IBOutlet IPCurveStorage * curveStorage;
}

@end
