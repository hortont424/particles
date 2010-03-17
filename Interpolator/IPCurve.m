#import "IPCurve.h"

@implementation IPCurve

@synthesize name;

- (id)init
{
    self = [super init];

    if(self)
    {
        IPControlPoint * pt;
        controlPoints = [[NSMutableArray alloc] init];
        name = @"x";
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
    [controlPoint setParent:self];
}

- (void)removeControlPoint:(IPControlPoint *)controlPoint
{
    [controlPoints removeObject:controlPoint];
    [controlPoint setParent:nil];
}

// (De)Coding

- (id)initWithCoder:(NSCoder *)coder
{
    if((self = [super init]))
    {
        controlPoints = [coder decodeObjectForKey:@"controlPoints"];

        for(IPControlPoint * cp in controlPoints)
        {
            [cp setParent:self];
        }

        name = [coder decodeObjectForKey:@"name"];
    }

    return self;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
    [coder encodeObject:controlPoints forKey:@"controlPoints"];
    [coder encodeObject:name forKey:@"name"];
}

@end
