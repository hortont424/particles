#import "IPControlPoint.h"

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
        return NSZeroPoint; // This could be valid; need a better sentry
    }
    
    return controlPoints[index];
}

- (double)controlWeights:(uint)index
{
    if(index < 0 || index >= 2)
    {
        NSLog(@"Invalid control weight index: %d.", index);
        return 0.0; // This could be valid; need a better sentry
    }
    
    return controlWeights[index];
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
    
    // TODO: Update control points, if necessary
}

- (void)setControlPoint:(uint)index toPoint:(NSPoint)newPoint
{
    if(index < 0 || index >= 2)
    {
        NSLog(@"Invalid control point index: %d.", index);
        return;
    }
    
    controlPoints[index] = newPoint;
    
    switch(type)
    {
        IP_CONTROL_POINT_SMOOTH:
        IP_CONTROL_POINT_SMOOTH_SYMMETRIC:
            // TODO: keep other control point in sync
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
    
    controlWeights[index] = newWeight;
    
    switch(type)
    {
        IP_CONTROL_POINT_SMOOTH:
            break;
        IP_CONTROL_POINT_SMOOTH_SYMMETRIC:
            controlWeights[!index] = controlWeights[index];
            break;
        IP_CONTROL_POINT_CORNER:
            break;
        default:
            NSLog(@"Invalid control point type: %d.", type);
            return;
    }
}

@end
