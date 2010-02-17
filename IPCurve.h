#import <Cocoa/Cocoa.h>

#import "IPControlPoint.h"

@interface IPCurve : NSObject
{
    NSMutableArray * controlPoints;
}

// Accessors
- (NSArray *)controlPoints;

// Mutators
- (void)addControlPoint:(IPControlPoint *)controlPoint;
- (void)removeControlPoint:(IPControlPoint *)controlPoint;

// (De)Coding
- (id)initWithCoder:(NSCoder*)coder;
- (void)encodeWithCoder:(NSCoder*)coder;

@end
