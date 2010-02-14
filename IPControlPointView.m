#import "IPControlPointView.h"

@implementation IPControlPointView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self)
    {
        IPControlPoint * pt;
        
        controlPointSubareas = [[NSMapTable alloc]
            initWithKeyOptions:NSMapTableStrongMemory
            valueOptions:NSMapTableStrongMemory
            capacity:32];
        
        controlPoints = [[NSMutableArray alloc] init];
        highlightedControlPoint = nil;
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(50, 50)];
        [pt setType:IP_CONTROL_POINT_CORNER];
        [pt setControlPoint:0 toPoint:NSMakePoint(15, 15)];
        [pt setControlPoint:1 toPoint:NSMakePoint(-2, -12)];
        
        [controlPoints addObject:pt];
        [self createTrackingAreasForControlPoint:pt inside:NO];
    }
    return self;
}

- (void)createTrackingAreasForControlPoint:(IPControlPoint *)controlPoint
    inside:(BOOL)inside
{
    NSTrackingAreaOptions trackingOptions;
    trackingOptions = (NSTrackingMouseEnteredAndExited | 
                       NSTrackingMouseMoved |
                       NSTrackingActiveInActiveApp |
                       NSTrackingEnabledDuringMouseDrag);
    
    NSPoint pt;
    NSRect rect;
    NSMutableDictionary * userInfo;
    NSTrackingArea * ta;
    NSMutableArray * subareas = [[NSMutableArray alloc] init];

    for(int index = 0; index <= 2; index++)
    {
        userInfo = [[NSMutableDictionary alloc] init];
        [userInfo setObject:controlPoint forKey:@"point"];
        [userInfo setObject:[NSNumber numberWithInt:index] forKey:@"subpoint"];
        
        if(index < 2)
        {
            pt = [controlPoint absoluteControlPoint:index];
            rect = NSRectAroundPoint(pt, 2, 2);
        }
        else
        {
            pt = [controlPoint point];
            rect = NSRectAroundPoint(pt, 3, 3);
            [controlPointSubareas setObject:subareas forKey:controlPoint];
        }
        
        if(index == 2 && inside)
        {
            trackingOptions = trackingOptions | NSTrackingAssumeInside;
        }
        
        ta = [[NSTrackingArea alloc] initWithRect:rect options:trackingOptions
            owner:self userInfo:userInfo];
        
        [self addTrackingArea:ta];
        [subareas addObject:ta];
    }
}

- (void)mouseDown:(NSEvent *) event
{
    selectedControlPoint = highlightedControlPoint;
    selectedSubpoint = highlightedSubpoint;
    dragPoint = [NSEvent mouseLocation];
    [self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent *) event
{
    dragPoint = NSFarAwayPoint;
    
    if(selectedControlPoint == nil)
        return;
    
    NSArray * areas = [controlPointSubareas objectForKey:selectedControlPoint];
    for(NSTrackingArea * ta in areas)
    {
        [self removeTrackingArea:ta];
    }
    
    [self createTrackingAreasForControlPoint:selectedControlPoint inside:YES];
}

- (void)mouseEntered:(NSEvent *)event
{
    NSDictionary * userInfo = [event userData];
    highlightedControlPoint = [userInfo objectForKey:@"point"];
    highlightedSubpoint = [[userInfo objectForKey:@"subpoint"] intValue];
    [self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event
{
    if(selectedControlPoint == nil)
        return;
    
    NSPoint delta = NSSubtractPoints([NSEvent mouseLocation], dragPoint);
    NSPoint point;
    
    if(selectedSubpoint == 2)
    {
        point = [selectedControlPoint point];
        point = NSAddPoints(point, delta);
        [selectedControlPoint setPoint:point];
    }
    else
    {
        point = [selectedControlPoint absoluteControlPoint:selectedSubpoint];
        point = NSAddPoints(point, delta);
        [selectedControlPoint setAbsoluteControlPoint:selectedSubpoint
            toPoint:point];
    }
    
    [self setNeedsDisplay:YES];
    
    dragPoint = [NSEvent mouseLocation];
}

- (void)mouseExited:(NSEvent *)event
{
    if(NSComparePoint(dragPoint, NSFarAwayPoint))
        highlightedControlPoint = nil;
    
    [self setNeedsDisplay:YES];
}

- (void)drawPoint:(IPControlPoint *)controlPoint
{
    CGContextRef ctx = [[NSGraphicsContext currentContext] graphicsPort];
    
    NSPoint point = [controlPoint point];
    CGRect ell;
    
    // Draw handles and control points
    for(int index = 0; index <= 2; index++)
    {
        NSPoint subpoint;
        
        if(controlPoint == selectedControlPoint &&
           selectedSubpoint == index)
        {
            CGContextSetRGBStrokeColor(ctx, 0.6, 0.6, 0.2, 1.0);
            CGContextSetRGBFillColor(ctx, 1.0, 1.0, 0.8, 1.0);
            CGContextSetLineWidth(ctx, 1.5);
        }
        else if(controlPoint == highlightedControlPoint &&
                highlightedSubpoint == index)
        {
            CGContextSetRGBStrokeColor(ctx, 0.2, 0.2, 0.6, 1.0);
            CGContextSetRGBFillColor(ctx, 0.6, 0.6, 1.0, 1.0);
            CGContextSetLineWidth(ctx, 1.5);
        }
        else
        {
            CGContextSetRGBFillColor(ctx, 0.8, 0.8, 0.8, 1.0);
            CGContextSetRGBStrokeColor(ctx, 0.2, 0.2, 0.2, 1.0);
            CGContextSetLineWidth(ctx, 1.0);
        }
        
        if(index < 2)
        {
            subpoint = [controlPoint absoluteControlPoint:index];
            ell = CGRectAroundPoint(subpoint, 2, 2);
        }
        else
        {
            subpoint = point;
            ell = CGRectAroundPoint(subpoint, 3, 3);
        }
    
        // Draw handles
        if(index < 2)
        {
            CGContextMoveToPoint(ctx, point.x, point.y);
            CGContextAddLineToPoint(ctx, subpoint.x, subpoint.y);
            CGContextStrokePath(ctx);
        }
    
        CGContextAddEllipseInRect(ctx, ell);
        CGContextFillPath(ctx);
        CGContextAddEllipseInRect(ctx, ell);
        CGContextStrokePath(ctx);
    }
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
