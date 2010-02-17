#import <Cocoa/Cocoa.h>
#import <ApplicationServices/ApplicationServices.h>

#import "IPCurveStorage.h"
#import "IPControlPoint.h"
#import "IPCurve.h"

@interface IPControlPointSelection : NSObject
{
    IPControlPoint * controlPoint;
    int subpoint;
}

@property (retain) IPControlPoint * controlPoint;
@property (assign) int subpoint;

@end

@interface IPCurveView : NSView
{
    NSMutableArray * curves;
    NSMapTable * controlPointSubareas;
    
    NSMutableArray * selection;
    
    IPControlPoint * highlightedControlPoint;
    int highlightedSubpoint;
    
    NSPoint dragPoint, boxPoint;
    
    BOOL drawControlPoints;
    
    IBOutlet IPCurveStorage * curveStorage;
}

- (void)createTrackingAreasForControlPoint:(IPControlPoint *)controlPoint;
- (void)updateCurves;

// Drawing
- (void)drawPoint:(IPControlPoint *)controlPoint;
- (void)drawCurve:(IPCurve *)curve;

// Accessors
- (BOOL)drawControlPoints;

// Mutators
- (void)setDrawControlPoints:(BOOL)newDrawControlPoints;

@end
