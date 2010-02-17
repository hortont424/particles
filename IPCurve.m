#import "IPCurve.h"

@implementation IPCurve

- (id)init
{
    self = [super init];
    
    if (self)
    {
        IPControlPoint * pt;
        
        controlPoints = [[NSMutableArray alloc] init];

        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(50, 50)];
        [pt setType:IP_CONTROL_POINT_CORNER];
        [pt setControlPoint:0 toPoint:NSMakePoint(-2, -12)];
        [pt setControlPoint:1 toPoint:NSMakePoint(15, 15)];
        
        [controlPoints addObject:pt];
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(150, 150)];
        [pt setType:IP_CONTROL_POINT_SMOOTH];
        [pt setControlPoint:0 toPoint:NSMakePoint(30, 15)];
        [pt setControlPoint:1 toPoint:NSMakePoint(0, -12)];
        
        [controlPoints addObject:pt];
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(300, 200)];
        [pt setType:IP_CONTROL_POINT_SMOOTH_SYMMETRIC];
        [pt setControlPoint:0 toPoint:NSMakePoint(-30, -30)];
        [pt setControlPoint:1 toPoint:NSMakePoint(30, 30)];
        
        [controlPoints addObject:pt];
    }
    return self;
}

// Accessors

- (NSArray *)controlPoints
{
    return controlPoints;
}

@end
