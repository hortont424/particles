#import <Cocoa/Cocoa.h>

#import "NSPointFunctions_CocoaDevUsersAdditions.h"

typedef enum
{
    IP_CONTROL_POINT_SMOOTH,
    IP_CONTROL_POINT_SMOOTH_SYMMETRIC,
    IP_CONTROL_POINT_CORNER,

    IP_CONTROL_POINT_SENTRY
} IPControlPointType;

@interface IPControlPoint : NSObject <NSCoding>
{
    NSPoint point;
    IPControlPointType type;

    NSPoint controlPoints[2];
}

// Accessors
- (NSPoint)point;
- (IPControlPointType)type;
- (NSPoint)controlPoint:(uint)index;
- (NSPoint)absoluteControlPoint:(uint)index;
- (double)controlWeights:(uint)index;

// Mutators
- (void)setPoint:(NSPoint)newPoint;
- (void)setType:(IPControlPointType)newType;
- (void)setControlPoint:(uint)index toPoint:(NSPoint)newPoint;
- (void)setAbsoluteControlPoint:(uint)index toPoint:(NSPoint)newPoint;
- (void)setControlWeight:(uint)index toValue:(double)newWeight;

// (De)Coding
- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

@end
