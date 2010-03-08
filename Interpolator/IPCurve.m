#import "IPCurve.h"

@implementation IPCurve

- (id)init
{
    self = [super init];

    if(self)
    {
        controlPoints = [[NSMutableArray alloc] init];
    }
    return self;
}

// Accessors

- (NSArray *)controlPoints
{
    return controlPoints;
}

// Mutators

- (void)addControlPoint:(IPControlPoint *)controlPoint
{
    [controlPoints addObject:controlPoint];
}

- (void)removeControlPoint:(IPControlPoint *)controlPoint
{
    [controlPoints removeObject:controlPoint];
}

// (De)Coding

- (id)initWithCoder:(NSCoder *)coder
{
    if((self = [super init]))
    {
        controlPoints = [coder decodeObjectForKey:@"controlPoints"];
    }

    return self;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
    [coder encodeObject:controlPoints forKey:@"controlPoints"];
}

@end
