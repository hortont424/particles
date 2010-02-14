#import "IPControlPoint.h"
#import "NSPointFunctions_CocoaDevUsersAdditions.h"

@implementation IPControlPoint

// Accessors

- (NSPoint)point
{
    return point;
}

- (IPControlPointType)type
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

- (void)setType:(IPControlPointType)newType
{
    if(newType < 0 || newType >= IP_CONTROL_POINT_SENTRY)
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
    
    switch(type)
    {
        IP_CONTROL_POINT_SMOOTH:
            otherPolarPoint.y = thisPolarPoint.y + M_PI;
            controlPoints[!index] = NSPolarToCartesian(otherPolarPoint);
            break;
        IP_CONTROL_POINT_SMOOTH_SYMMETRIC:
            otherPolarPoint.x = thisPolarPoint.x;
            otherPolarPoint.y = thisPolarPoint.y + M_PI;
            controlPoints[!index] = NSPolarToCartesian(otherPolarPoint);
            break;
        IP_CONTROL_POINT_CORNER:
            break;
        default:
            NSLog(@"Invalid control point type: %d.", type);
            return;
    }
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
        IP_CONTROL_POINT_SMOOTH:
            break;
        IP_CONTROL_POINT_SMOOTH_SYMMETRIC:
            otherPolarPoint.x = newWeight;
            controlPoints[!index] = NSPolarToCartesian(otherPolarPoint);
            break;
        IP_CONTROL_POINT_CORNER:
            break;
        default:
            NSLog(@"Invalid control point type: %d.", type);
            return;
    }
}

@end
