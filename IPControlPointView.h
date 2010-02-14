#import <Cocoa/Cocoa.h>
#import <ApplicationServices/ApplicationServices.h>

#import "IPControlPoint.h"

@interface IPControlPointView : NSView
{
    NSMutableArray * controlPoints;
    NSMapTable * controlPointSubareas;
    
    IPControlPoint * selectedControlPoint;
    int selectedSubpoint;
    
    IPControlPoint * highlightedControlPoint;
    int highlightedSubpoint;
    
    NSPoint dragPoint;
}

- (void)createTrackingAreasForControlPoint:(IPControlPoint *)controlPoint;
- (void)drawPoint:(IPControlPoint *)controlPoint;

@end
