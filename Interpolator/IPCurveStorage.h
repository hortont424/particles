#import <Cocoa/Cocoa.h>

#import "IPCurve.h"
#import "IPControlPoint.h"
#import "IPCurveSet.h"

@interface IPCurveStorage : NSObject
{
    NSMutableArray * curveSets;

    IBOutlet id delegate;
}

- (NSString *)pathForDataFile;

// Mutator
- (IPCurveSet *)addCurveSet;

// Accessor
- (NSMutableArray *)curveSets;

// (De)Coding
- (void)save;
- (void)load;

@property (nonatomic,retain) IBOutlet id delegate;

@end
