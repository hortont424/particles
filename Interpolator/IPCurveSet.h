#import <Cocoa/Cocoa.h>

#import "IPCurve.h"

@interface IPCurveSet : NSObject <NSCoding>
{
    NSString * name;
    NSMutableArray * curves;
}

// Mutator
- (void)addCurve:(IPCurve *)curve;

// (De)Coding
- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

@property (nonatomic,retain) NSString * name;
@property (nonatomic,retain) NSMutableArray * curves;

@end
