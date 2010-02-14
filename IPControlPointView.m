#import "IPControlPointView.h"

@implementation IPControlPointView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self)
    {
        IPControlPoint * pt;
        
        controlPoints = [[NSMutableArray alloc] init];
        highlightedControlPoint = nil;
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(50, 50)];
        [pt setType:IP_CONTROL_POINT_CORNER];
        [pt setControlPoint:0 toPoint:NSMakePoint(15, 15)];
        [pt setControlPoint:1 toPoint:NSMakePoint(-2, -12)];
        
        [controlPoints addObject:pt];
        NSRect rect = NSMakeRect([pt point].x, [pt point].y, 0.0, 0.0);
        rect = NSInsetRect(rect, -3, -3);
        // TODO: UNMESSIFY
        NSTrackingArea * ta;
        ta = [[NSTrackingArea alloc] initWithRect:rect
            options:(NSTrackingMouseEnteredAndExited | 
                     NSTrackingMouseMoved |
                     NSTrackingActiveInActiveApp)
            owner:self
            userInfo:[NSDictionary dictionaryWithObjectsAndKeys:pt,@"point",
                    [NSNumber numberWithInt:-1], @"subpoint", nil]];
        [self addTrackingArea:ta];
    }
    return self;
}

- (void) mouseDown: (NSEvent *) event
{
    selectedControlPoint = highlightedControlPoint;
    [self setNeedsDisplay:YES];
}

- (void)mouseEntered:(NSEvent *)event
{
    highlightedControlPoint = [[event userData] objectForKey:@"point"];
    [self setNeedsDisplay:YES];
}

- (void)mouseExited:(NSEvent *)event
{
    highlightedControlPoint = nil;
    [self setNeedsDisplay:YES];
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
    
    // Draw handles and sub-control-points
    for(int index = 0; index < 2; index++)
    {
        NSPoint subpoint = [controlPoint absoluteControlPoint:index];
        CGRect ell = CGRectMake(subpoint.x, subpoint.y, 0.0, 0.0);
        ell = CGRectInset(ell, -2, -2);
    
        CGContextMoveToPoint(ctx, point.x, point.y);
        CGContextAddLineToPoint(ctx, subpoint.x, subpoint.y);
        CGContextStrokePath(ctx);
    
        CGContextAddEllipseInRect(ctx, ell);
        CGContextFillPath(ctx);
        CGContextAddEllipseInRect(ctx, ell);
        CGContextStrokePath(ctx);
    }
    
    if(controlPoint == selectedControlPoint)
    {
        CGContextSetRGBStrokeColor(ctx, 0.6, 0.6, 0.2, 1.0);
        CGContextSetRGBFillColor(ctx, 1.0, 1.0, 0.8, 1.0);
        CGContextSetLineWidth(ctx, 1.5);
    }
    else if(controlPoint == highlightedControlPoint)
    {
        CGContextSetRGBStrokeColor(ctx, 0.2, 0.2, 0.6, 1.0);
        CGContextSetRGBFillColor(ctx, 0.6, 0.6, 1.0, 1.0);
        CGContextSetLineWidth(ctx, 1.5);
    }
    
    // Draw actual control point
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
