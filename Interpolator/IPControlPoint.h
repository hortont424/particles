#import <Cocoa/Cocoa.h>

#import "NSPointFunctions_CocoaDevUsersAdditions.h"
#import "libcurve/Curve.h"

@interface IPControlPoint : NSObject <NSCoding>
{
    NSPoint point;
    CUControlPointType type;

    NSPoint controlPoints[2];
}

// Accessors
- (NSPoint)point;
- (CUControlPointType)type;
- (NSPoint)controlPoint:(uint)index;
- (NSPoint)absoluteControlPoint:(uint)index;
- (double)controlWeights:(uint)index;

// Mutators
- (void)setPoint:(NSPoint)newPoint;
- (void)setType:(CUControlPointType)newType;
- (void)setControlPoint:(uint)index toPoint:(NSPoint)newPoint;
- (void)setAbsoluteControlPoint:(uint)index toPoint:(NSPoint)newPoint;
- (void)setControlWeight:(uint)index toValue:(double)newWeight;

// (De)Coding
- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

@end
