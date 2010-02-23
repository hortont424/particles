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
    unsigned int curvesIndex;
    NSMapTable * controlPointSubareas;
    
    NSMutableArray * selection;
    
    // TODO: use IPControlPointSelection for highlight too!
    IPControlPoint * nextHighlightedControlPoint;
    int nextHighlightedSubpoint;
    BOOL nextHighlightedControlPointSet;
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
- (unsigned int)curvesIndex;

// Mutators
- (void)setDrawControlPoints:(BOOL)newDrawControlPoints;
- (void)setCurvesIndex:(unsigned int)newCurvesIndex;

@end
