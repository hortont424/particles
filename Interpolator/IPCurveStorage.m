#import "IPCurveStorage.h"
#import "IPCurveView.h"

@implementation IPCurveStorage

@synthesize curveView; // TODO: Yuck

- (id)init
{
    self = [super init];
    
    if(self)
    {
        curveSets = [[NSMutableArray alloc] init];
    }
    
    return self;
}

- (NSString *)pathForDataFile
{
    NSFileManager * fileManager = [NSFileManager defaultManager];
    NSString * folder = @"~/Library/Application Support/Interpolator/";
    folder = [folder stringByExpandingTildeInPath];
    
    if(![fileManager fileExistsAtPath:folder])
        [fileManager createDirectoryAtPath:folder
            withIntermediateDirectories:YES attributes:nil error:NULL];
    
    // TODO: check for error
    
    NSString * fileName = @"interpolator.curves";
    
    return [folder stringByAppendingPathComponent:fileName];
}

// Accessors

- (NSMutableArray *)curveSets
{
    return curveSets;
}

// (De)Coding

- (void)save
{
    NSDictionary * root;
    root = [NSDictionary dictionaryWithObject:curveSets forKey:@"curveSets"];
    [NSKeyedArchiver archiveRootObject:root toFile:[self pathForDataFile]];
}

- (void)load
{
    NSDictionary * root;
    
    root = [NSKeyedUnarchiver unarchiveObjectWithFile:[self pathForDataFile]];
    
    if(root)
    {
        curveSets = [root objectForKey:@"curveSets"];
    }
    else
    {
        // Add one default curve
        IPCurveSet * curveSet = [[IPCurveSet alloc] init];
        IPCurve * curve = [[IPCurve alloc] init];
        IPControlPoint * pt;

        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(50, 50)];
        [pt setType:IP_CONTROL_POINT_CORNER];
        [pt setControlPoint:0 toPoint:NSMakePoint(-2, -12)];
        [pt setControlPoint:1 toPoint:NSMakePoint(15, 15)];
        [curve addControlPoint:pt];
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(150, 150)];
        [pt setType:IP_CONTROL_POINT_SMOOTH];
        [pt setControlPoint:0 toPoint:NSMakePoint(30, 15)];
        [pt setControlPoint:1 toPoint:NSMakePoint(0, -12)];
        [curve addControlPoint:pt];
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(300, 200)];
        [pt setType:IP_CONTROL_POINT_SMOOTH_SYMMETRIC];
        [pt setControlPoint:0 toPoint:NSMakePoint(-30, -30)];
        [pt setControlPoint:1 toPoint:NSMakePoint(30, 30)];
        [curve addControlPoint:pt];
        
        [curveSet addCurve:curve];
        
        [curveSets addObject:curveSet];
    }
    
    [curveView updateCurves];
}

@end