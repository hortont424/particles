#import <Cocoa/Cocoa.h>

#import "IPControlPoint.h"

@interface IPCurve : NSObject
{
    NSMutableArray * controlPoints;
    NSString * name;
}

// Accessors
- (NSArray *)controlPoints;

// Mutators
- (void)addControlPoint:(IPControlPoint *)controlPoint;
- (void)removeControlPoint:(IPControlPoint *)controlPoint;
- (void)setControlPoints:(NSArray *)cps;

// (De)Coding
- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

@property (nonatomic,retain) NSString * name;

@end
