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
        
        dragPoint = boxPoint = NSFarAwayPoint;
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
    NSPoint mouse = [self convertPointFromBase:
        [[NSApp keyWindow] mouseLocationOutsideOfEventStream]];
    
    BOOL appendSelection = ([event modifierFlags] & NSShiftKeyMask) != 0;
    
    if(highlightedControlPoint == nil)
    {
        boxPoint = mouse;
    }
    else
    {
        dragPoint = mouse;
        
        // Make sure point hasn't already been added to selection
        for(IPControlPointSelection * sel in selection)
        {
            if(sel.controlPoint == highlightedControlPoint &&
               sel.subpoint == highlightedSubpoint)
               return;
        }
        
        BOOL selectingHandles =
            ((IPControlPointSelection *)[selection lastObject]).subpoint != 2;
        
        // Multiple selection only works on big control points, not handles
        // So, if we're selecting a handle, or have handles selected,
        // we have to reset the selection. Also, if the shift key isn't
        // held down, we have to reset the selection.
        if(highlightedSubpoint != 2 || selectingHandles || !appendSelection)
        {
            [selection removeAllObjects];
        }
        
        // Add new control point
        IPControlPointSelection * sel = [[IPControlPointSelection alloc] init];
        sel.controlPoint = highlightedControlPoint;
        sel.subpoint = highlightedSubpoint;
        [selection addObject:sel];
        
        // Remove tracking areas for all control points
        // They'll be readded when the user releases the mouse
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
    BOOL appendSelection = ([event modifierFlags] & NSShiftKeyMask) != 0;
    
    if(!NSComparePoint(boxPoint, NSFarAwayPoint))
    {
        NSPoint mouse = [self convertPointFromBase:
            [[NSApp keyWindow] mouseLocationOutsideOfEventStream]];
        
        if(!appendSelection ||
           ((IPControlPointSelection *)[selection lastObject]).subpoint != 2)
            [selection removeAllObjects];
        
        // Add control points within box select to selection
        for(IPControlPoint * controlPoint in controlPoints)
        {
            if(NSPointInRect([controlPoint point],
                NSRectWithPoints(boxPoint, mouse)))
            {
                BOOL pointAlreadySelected = NO;
                
                // Make sure control point isn't already selected
                for(IPControlPointSelection * sel in selection)
                {
                    if(sel.controlPoint == controlPoint &&
                       sel.subpoint == 2)
                    {
                        pointAlreadySelected = YES;
                        break;
                    }
                }
                
                if(pointAlreadySelected)
                    continue;
                
                IPControlPointSelection * sel = 
                    [[IPControlPointSelection alloc] init];
                sel.controlPoint = controlPoint;
                sel.subpoint = 2;
                [selection addObject:sel];
            }
        }
        
        boxPoint = NSFarAwayPoint;
        [self setNeedsDisplay:YES];
        return;
    }
    
    dragPoint = NSFarAwayPoint;
    
    if([selection count])
    {
        [NSCursor unhide];
        
        // Recalculate and add new tracking areas for control points
        for(IPControlPointSelection * sel in selection)
        {
            [self createTrackingAreasForControlPoint:sel.controlPoint];
        }
    }
    
    [self setNeedsDisplay:YES];
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
    // If we're doing box select, don't update any control points
    if(!NSComparePoint(boxPoint, NSFarAwayPoint))
    {
        [self setNeedsDisplay:YES];
        return;
    }
    
    if([selection count] == 0 || NSComparePoint(dragPoint, NSFarAwayPoint))
        return;
    
    BOOL snapAngle = ([event modifierFlags] & NSShiftKeyMask) != 0;
    
    NSPoint mouse = [self convertPointFromBase:
        [[NSApp keyWindow] mouseLocationOutsideOfEventStream]];
    
    NSPoint delta = NSSubtractPoints(mouse, dragPoint);
    NSPoint point;
    
    // Shift all selected control points by delta
    for(IPControlPointSelection * sel in selection)
    {
        IPControlPoint * controlPoint = sel.controlPoint;
        int subpoint = sel.subpoint;
        
        if(subpoint == 2)
            point = [controlPoint point];
        else
            point = [controlPoint controlPoint:subpoint];
            
        point = NSAddPoints(point, delta);
        
        // Snap angle to 45s
        if(snapAngle && subpoint != 2)
        {
            NSPoint polar = NSCartesianToPolar(point);
            
            double diff = 100.0;
            double ideal = 0.0;
            
            for(double angle = -M_PI; angle <= M_PI; angle += M_PI / 4.0)
            {
                if(fabs(polar.y - angle) < diff)
                {
                    diff = fabs(polar.y - angle);
                    ideal = angle;
                }
            }
            
            polar.y = ideal;

            point = NSPolarToCartesian(polar);
        }
        
        if(subpoint == 2)
            [controlPoint setPoint:point];
        else
            [controlPoint setControlPoint:subpoint toPoint:point];
    }
    
    [self setNeedsDisplay:YES];
    
    dragPoint = mouse;
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
    
    // Find our control point, if it's selected
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
    
    // Draw box selector
    if(!NSComparePoint(boxPoint, NSFarAwayPoint))
    {
        NSColor * blue = [NSColor colorWithCalibratedRed:0.447 green:0.624
            blue: 0.812 alpha:0.5];
            
        [[blue colorWithAlphaComponent:0.2] setFill];
        [blue setStroke];
        
        NSPoint mouse = [self convertPointFromBase:
            [[NSApp keyWindow] mouseLocationOutsideOfEventStream]];
        [NSBezierPath fillRect:NSRectWithPoints(boxPoint, mouse)];
        [NSBezierPath strokeRect:NSRectWithPoints(boxPoint, mouse)];
    }
}

@end
