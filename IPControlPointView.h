#import <Cocoa/Cocoa.h>
#import <ApplicationServices/ApplicationServices.h>

#import "IPControlPoint.h"

@interface IPControlPointView : NSView
{
    NSMutableArray * controlPoints;
    IPControlPoint * selectedControlPoint;
    IPControlPoint * highlightedControlPoint;
}

@end
