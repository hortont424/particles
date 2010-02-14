#import <Cocoa/Cocoa.h>

typedef enum
{
    IP_CONTROL_POINT_SMOOTH,
    IP_CONTROL_POINT_SMOOTH_SYMMETRIC,
    IP_CONTROL_POINT_CORNER,
    
    IP_CONTROL_POINT_SENTRY
} IPControlPointType;

@interface IPControlPoint : NSObject
{
    NSPoint point;
    IPControlPointType type;
    
    NSPoint controlPoints[2];
    double controlWeights[2];
}

@end
