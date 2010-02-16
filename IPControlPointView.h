#import <Cocoa/Cocoa.h>
#import <ApplicationServices/ApplicationServices.h>

#import "IPControlPoint.h"

@interface IPControlPointSelection : NSObject
{
    IPControlPoint * controlPoint;
    int subpoint;
}

@property (retain) IPControlPoint * controlPoint;
@property (assign) int subpoint;

@end

@interface IPControlPointView : NSView
{
    NSMutableArray * controlPoints;
    NSMapTable * controlPointSubareas;
    
    NSMutableArray * selection;
    
    IPControlPoint * highlightedControlPoint;
    int highlightedSubpoint;
    
    NSPoint dragPoint, boxPoint;
    
    BOOL drawControlPoints;
}

- (void)createTrackingAreasForControlPoint:(IPControlPoint *)controlPoint;
- (void)drawPoint:(IPControlPoint *)controlPoint;

- (void)setDrawControlPoints:(BOOL)newDrawControlPoints;
- (BOOL)drawControlPoints;

@end
