#import <Cocoa/Cocoa.h>

#import "IPCurve.h"
#import "IPControlPoint.h"

@interface IPCurveStorage : NSObject
{
    NSMutableArray * curves;
	
    IBOutlet id curveView;
}

- (NSString *)pathForDataFile;

// Accessors
- (NSMutableArray *)curves;

// (De)Coding
- (void)save;
- (void)load;

@property (nonatomic,retain) IBOutlet id curveView;

@end
