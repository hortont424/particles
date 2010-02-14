#import "IPControlPointView.h"

@implementation IPControlPointView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self)
    {
        IPControlPoint * pt;
        
        controlPoints = [[NSMutableArray alloc] init];
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(50, 50)];
        [pt setType:IP_CONTROL_POINT_CORNER];
        [pt setControlPoint:0 toPoint:NSMakePoint(10, 10)];
        [pt setControlPoint:1 toPoint:NSMakePoint(-2, -10)];
        
        [controlPoints addObject:pt];
    }
    return self;
}

- (void)drawPoint:(IPControlPoint *)controlPoint
{
    CGContextRef ctx = [[NSGraphicsContext currentContext] graphicsPort];
    
    NSPoint point = [controlPoint point];
    CGRect ell = CGRectMake(point.x, point.y, 0.0, 0.0);
    ell = CGRectInset(ell, -3, -3);
    
    CGContextSetRGBFillColor(ctx, 0.8, 0.8, 0.8, 1.0);
    CGContextSetRGBStrokeColor(ctx, 0.2, 0.2, 0.2, 1.0);
    CGContextSetLineWidth(ctx, 1.0);
    
    CGContextAddEllipseInRect(ctx, ell);
    CGContextFillPath(ctx);
    CGContextAddEllipseInRect(ctx, ell);
    CGContextStrokePath(ctx);
}

- (void)drawRect:(NSRect)dirtyRect
{
    [[NSColor whiteColor] setFill];
    NSRectFill(dirtyRect);
    
    for(IPControlPoint * controlPoint in controlPoints)
    {
        [self drawPoint:controlPoint];
    }
}

@end
