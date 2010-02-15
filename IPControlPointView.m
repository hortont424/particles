#import "IPControlPointView.h"

@implementation IPControlPointSelection
@synthesize controlPoint, subpoint;
@end

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
        
        dragPoint = NSFarAwayPoint;
        controlPoints = [[NSMutableArray alloc] init];
        highlightedControlPoint = nil;
        selection = [[NSMutableArray alloc] init];
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(50, 50)];
        [pt setType:IP_CONTROL_POINT_CORNER];
        [pt setControlPoint:0 toPoint:NSMakePoint(15, 15)];
        [pt setControlPoint:1 toPoint:NSMakePoint(-2, -12)];
        
        [controlPoints addObject:pt];
        [self createTrackingAreasForControlPoint:pt];
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(150, 150)];
        [pt setType:IP_CONTROL_POINT_SMOOTH];
        [pt setControlPoint:0 toPoint:NSMakePoint(30, 15)];
        [pt setControlPoint:1 toPoint:NSMakePoint(0, -12)];
        
        [controlPoints addObject:pt];
        [self createTrackingAreasForControlPoint:pt];
        
        pt = [[IPControlPoint alloc] init];
        [pt setPoint:NSMakePoint(300, 200)];
        [pt setType:IP_CONTROL_POINT_SMOOTH_SYMMETRIC];
        [pt setControlPoint:0 toPoint:NSMakePoint(30, 30)];
        [pt setControlPoint:1 toPoint:NSMakePoint(-30, -30)];
        
        [controlPoints addObject:pt];
        [self createTrackingAreasForControlPoint:pt];
    }
    return self;
}

- (void)createTrackingAreasForControlPoint:(IPControlPoint *)controlPoint
{
    NSTrackingAreaOptions trackingOptions;
    trackingOptions = (NSTrackingMouseEnteredAndExited | 
                       NSTrackingMouseMoved |
                       NSTrackingActiveInActiveApp |
                       NSTrackingEnabledDuringMouseDrag |
                       NSTrackingCursorUpdate);
    
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
        
        if(index != 2)
        {
            pt = [controlPoint absoluteControlPoint:index];
            rect = NSRectAroundPoint(pt, 4, 4);
        }
        else
        {
            pt = [controlPoint point];
            rect = NSRectAroundPoint(pt, 6, 6);
            [controlPointSubareas setObject:subareas forKey:controlPoint];
        }
        
        ta = [[NSTrackingArea alloc] initWithRect:rect options:trackingOptions
            owner:self userInfo:userInfo];
        
        [self addTrackingArea:ta];
        [subareas addObject:ta];
    }
}

- (void)cursorUpdate:(NSEvent *)event
{
    NSPoint hitPoint;
    NSTrackingArea *trackingArea;

    trackingArea = [event trackingArea];
    hitPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    
    if ([self mouse:hitPoint inRect:[trackingArea rect]] ||
        highlightedControlPoint)
    {
        [[NSCursor crosshairCursor] set];
    }
    else
    {
        [[NSCursor arrowCursor] set];
    }
}

- (void)mouseDown:(NSEvent *) event
{
    if(highlightedControlPoint == nil)
    {
        [selection removeAllObjects];
    }
    else
    {
        dragPoint = [NSEvent mouseLocation];
        
        IPControlPointSelection * sel = [[IPControlPointSelection alloc] init];
        
        // Make sure point hasn't already been added to selection
        long selectionIndex = [selection indexOfObjectPassingTest:
            ^ BOOL (id obj, NSUInteger idx, BOOL * stop)
            {
                IPControlPointSelection * sel = (IPControlPointSelection *)obj;
                return (sel.controlPoint == highlightedControlPoint &&
                        sel.subpoint == highlightedSubpoint);
            }];
        
        if(selectionIndex >= 0 && selectionIndex != NSNotFound)
            return;
    
        sel.controlPoint = highlightedControlPoint;
        sel.subpoint = highlightedSubpoint;
        
        BOOL selectingHandles =
            ((IPControlPointSelection *)[selection lastObject]).subpoint != 2;
        
        BOOL appendSelection = ([event modifierFlags] & NSShiftKeyMask) != 0;
        
        if(highlightedSubpoint != 2 || selectingHandles || !appendSelection)
        {
            [selection removeAllObjects];
        }
        
        [selection addObject:sel];
        
        for(IPControlPointSelection * sel in selection)
        {
            NSArray * areas;
            areas = [controlPointSubareas objectForKey:sel.controlPoint];
            for(NSTrackingArea * ta in areas)
            {
                [self removeTrackingArea:ta];
            }
        }
        
        [NSCursor hide];
    }
    
    [self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent *) event
{
    dragPoint = NSFarAwayPoint;
    
    if([selection count])
    {
        [NSCursor unhide];
        
        for(IPControlPointSelection * sel in selection)
        {
            [self createTrackingAreasForControlPoint:sel.controlPoint];
        }
    }
}

- (void)mouseEntered:(NSEvent *)event
{
    if(!NSComparePoint(dragPoint, NSFarAwayPoint))
        return;
    
    NSDictionary * userInfo = [event userData];
    highlightedControlPoint = [userInfo objectForKey:@"point"];
    highlightedSubpoint = [[userInfo objectForKey:@"subpoint"] intValue];
    [self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event
{
    if([selection count] == 0)
        return;
    
    NSPoint delta = NSSubtractPoints([NSEvent mouseLocation], dragPoint);
    NSPoint point;
    
    for(IPControlPointSelection * sel in selection)
    {
        IPControlPoint * controlPoint = sel.controlPoint;
        int subpoint = sel.subpoint;
        
        if(subpoint == 2)
        {
            point = [controlPoint point];
            point = NSAddPoints(point, delta);
            [controlPoint setPoint:point];
        }
        else
        {
            point = [controlPoint absoluteControlPoint:subpoint];
            point = NSAddPoints(point, delta);
            [controlPoint setAbsoluteControlPoint:subpoint toPoint:point];
        }
    }

    [self setNeedsDisplay:YES];
    
    dragPoint = [NSEvent mouseLocation];
}

- (void)mouseExited:(NSEvent *)event
{
    if(!NSComparePoint(dragPoint, NSFarAwayPoint))
        return;
    
    highlightedControlPoint = nil;

    [self setNeedsDisplay:YES];
}

- (void)drawPoint:(IPControlPoint *)controlPoint
{
    CGContextRef ctx = [[NSGraphicsContext currentContext] graphicsPort];
    
    long selectionIndex = [selection indexOfObjectPassingTest:
        ^ BOOL (id obj, NSUInteger idx, BOOL * stop)
        {
            IPControlPointSelection * sel = (IPControlPointSelection *)obj;
            return (sel.controlPoint == controlPoint);
        }];
    
    IPControlPointSelection * sel = nil;
    
    if(selectionIndex >= 0 && selectionIndex != NSNotFound)
        sel = [selection objectAtIndex:selectionIndex];
    
    NSPoint point = [controlPoint point];
    CGRect ell;
    
    // Draw handles and control points
    for(int index = 0; index <= 2; index++)
    {
        NSPoint subpoint;
        
        if(sel && sel.subpoint == index)
        {
            CGContextSetRGBStrokeColor(ctx, 0.788, 0.714, 0.110, 1.0);
            CGContextSetRGBFillColor(ctx, 0.988, 0.914, 0.310, 1.0);
            CGContextSetLineWidth(ctx, 1.5);
        }
        else if(controlPoint == highlightedControlPoint &&
                highlightedSubpoint == index)
        {
            CGContextSetRGBStrokeColor(ctx, 0.247, 0.424, 0.612, 1.0);
            CGContextSetRGBFillColor(ctx, 0.447, 0.624, 0.812, 1.0);
            CGContextSetLineWidth(ctx, 1.5);
        }
        else
        {
            CGContextSetRGBFillColor(ctx, 0.8, 0.8, 0.8, 1.0);
            CGContextSetRGBStrokeColor(ctx, 0.2, 0.2, 0.2, 1.0);
            CGContextSetLineWidth(ctx, 1.0);
        }
        
        if(index != 2)
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
        if(index != 2)
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
