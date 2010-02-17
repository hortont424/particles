#import <Cocoa/Cocoa.h>

#import "IPControlPoint.h"

@interface IPCurve : NSObject
{
    NSMutableArray * controlPoints;
}

// Accessors
- (NSArray *)controlPoints;

// (De)Coding
- (id)initWithCoder:(NSCoder*)coder;
- (void)encodeWithCoder:(NSCoder*)coder;

@end
