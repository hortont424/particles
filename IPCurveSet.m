#import "IPCurveSet.h"

@implementation IPCurveSet

@synthesize name, curves;

- (id)init
{
    self = [super init];
    
    if(self)
    {
        name = @"Untitled Curveset";
        curves = [[NSMutableArray alloc] init];
    }
    
    return self;
}

// Mutators

- (void)addCurve:(IPCurve *)curve
{
    [curves addObject:curve];
}

// (De)Coding

- (id)initWithCoder:(NSCoder *)coder
{
    if(self = [super init])
    {
        name = [coder decodeObjectForKey:@"name"];
        curves = [coder decodeObjectForKey:@"curves"];
    }
    
    return self;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
    [coder encodeObject:name forKey:@"name"];
    [coder encodeObject:curves forKey:@"curves"];
}

@end
