#import "IPControlPoint.h"

@implementation IPControlPoint

@synthesize parent;

// Accessors

- (NSPoint)point
{
    return point;
}

- (CUControlPointType)type
{
    return type;
}

- (NSPoint)controlPoint:(uint)index
{
    if(index < 0 || index >= 2)
    {
        NSLog(@"Invalid control point index: %d.", index);
        return NSFarAwayPoint;
    }

    return controlPoints[index];
}

- (NSPoint)absoluteControlPoint:(uint)index
{
    if(index < 0 || index >= 2)
    {
        NSLog(@"Invalid control point index: %d.", index);
        return NSFarAwayPoint;
    }

    return NSAddPoints(point, controlPoints[index]);
}

- (double)controlWeights:(uint)index
{
    if(index < 0 || index >= 2)
    {
        NSLog(@"Invalid control weight index: %d.", index);
        return -1.0;
    }

    NSPoint polarPoint = NSCartesianToPolar(controlPoints[index]);
    return polarPoint.x;
}

// Mutators

- (void)setPoint:(NSPoint)newPoint
{
    point = newPoint;
}

- (void)setType:(CUControlPointType)newType
{
    if(newType < 0 || newType >= CU_CONTROL_POINT_SENTRY)
    {
        NSLog(@"Invalid control point type: %d.", newType);
        return;
    }

    type = newType;

    // Update control points, in case behavior changes
    [self setControlPoint:0 toPoint:[self controlPoint:0]];
    [self setControlPoint:1 toPoint:[self controlPoint:1]];
}

- (void)setControlPoint:(uint)index toPoint:(NSPoint)newPoint
{
    if(index < 0 || index >= 2)
    {
        NSLog(@"Invalid control point index: %d.", index);
        return;
    }

    controlPoints[index] = newPoint;

    NSPoint thisPolarPoint = NSCartesianToPolar(controlPoints[index]);
    NSPoint otherPolarPoint = NSCartesianToPolar(controlPoints[!index]);

    // Constrain left control point to left quadrants
    if(index == 0 &&
       thisPolarPoint.y < (M_PI / 2.0) && thisPolarPoint.y > 0)
        thisPolarPoint.y = (M_PI / 2.0);

    if(index == 0 &&
       thisPolarPoint.y < 0 && thisPolarPoint.y > -(M_PI / 2.0))
        thisPolarPoint.y = -(M_PI / 2.0);

    // Constrain right handle to right quadrants
    if(index == 1 &&
       thisPolarPoint.y > (M_PI / 2.0) && thisPolarPoint.y < M_PI)
        thisPolarPoint.y = (M_PI / 2.0);

    if(index == 1 &&
       thisPolarPoint.y < -(M_PI / 2.0) && thisPolarPoint.y > -M_PI)
        thisPolarPoint.y = -(M_PI / 2.0);

    switch(type)
    {
        case CU_CONTROL_POINT_SMOOTH:
            otherPolarPoint.y = thisPolarPoint.y + M_PI;
            break;
        case CU_CONTROL_POINT_SMOOTH_SYMMETRIC:
            otherPolarPoint.x = thisPolarPoint.x;
            otherPolarPoint.y = thisPolarPoint.y + M_PI;
            break;
        case CU_CONTROL_POINT_CORNER:
            break;
        default:
            NSLog(@"Invalid control point type: %d.", type);
            return;
    }

    controlPoints[index] = NSPolarToCartesian(thisPolarPoint);
    controlPoints[!index] = NSPolarToCartesian(otherPolarPoint);
}

- (void)setAbsoluteControlPoint:(uint)index toPoint:(NSPoint)newPoint
{
    if(index < 0 || index >= 2)
    {
        NSLog(@"Invalid control point index: %d.", index);
        return;
    }

    [self setControlPoint:index
        toPoint:NSSubtractPoints(newPoint, point)];
}

- (void)setControlWeight:(uint)index toValue:(double)newWeight
{
    if(index < 0 || index >= 2)
    {
        NSLog(@"Invalid control weight index: %d.", index);
        return;
    }

    NSPoint thisPolarPoint = NSCartesianToPolar(controlPoints[index]);
    NSPoint otherPolarPoint = NSCartesianToPolar(controlPoints[!index]);

    thisPolarPoint.x = newWeight;
    controlPoints[index] = NSPolarToCartesian(thisPolarPoint);

    switch(type)
    {
        case CU_CONTROL_POINT_SMOOTH:
            break;
        case CU_CONTROL_POINT_SMOOTH_SYMMETRIC:
            otherPolarPoint.x = newWeight;
            controlPoints[!index] = NSPolarToCartesian(otherPolarPoint);
            break;
        case CU_CONTROL_POINT_CORNER:
            break;
        default:
            NSLog(@"Invalid control point type: %d.", type);
            return;
    }
}

// (De)Coding

- (id)initWithCoder:(NSCoder *)coder
{
    if((self = [super init]))
    {
        point = [coder decodePointForKey:@"point"];
        type = [coder decodeIntForKey:@"type"];
        controlPoints[0] = [coder decodePointForKey:@"leftControlPoint"];
        controlPoints[1] = [coder decodePointForKey:@"rightControlPoint"];
    }

    return self;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
    [coder encodePoint:point forKey:@"point"];
    [coder encodeInt:type forKey:@"type"];
    [coder encodePoint:controlPoints[0] forKey:@"leftControlPoint"];
    [coder encodePoint:controlPoints[1] forKey:@"rightControlPoint"];
}

@end
