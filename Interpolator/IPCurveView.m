/* particles - interpolator - IPCurveView.m
 *
 * Copyright 2010 Tim Horton. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY TIM HORTON "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL TIM HORTON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "IPCurveView.h"
#import "IPController.h"

@implementation IPControlPointSelection
@synthesize controlPoint, subpoint;
@end

@implementation IPCurveView

@synthesize delegate;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];

    if (self)
    {
        drawControlPoints = TRUE;

        controlPointSubareas = [[NSMapTable alloc]
            initWithKeyOptions:NSMapTableStrongMemory
            valueOptions:NSMapTableStrongMemory
            capacity:32];

        dragPoint = boxPoint = NSFarAwayPoint;
        highlightedControlPoint = nil;
        selection = nil;

        [self clearSelection];
    }
    return self;
}

- (void)updateCurves
{
    curves = [displayCurveSet curves];

    for(NSTrackingArea * ta in [self trackingAreas])
        [self removeTrackingArea:ta];

    for(IPCurve * curve in curves)
        for(IPControlPoint * pt in [curve controlPoints])
            [self createTrackingAreasForControlPoint:pt];

    [self setNeedsDisplay:YES];
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
        [NSCursor hide];

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
            [self clearSelection];
        }

        // Add new control point
        IPControlPointSelection * sel = [[IPControlPointSelection alloc] init];
        sel.controlPoint = highlightedControlPoint;
        sel.subpoint = highlightedSubpoint;
        [selection addObject:sel];
        [self updateSelection];
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
            [self clearSelection];

        // Add control points within box select to selection
        for(IPCurve * curve in curves)
        {
            for(IPControlPoint * controlPoint in [curve controlPoints])
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
                    [self updateSelection];
                }
            }
        }
        boxPoint = NSFarAwayPoint;
        [self setNeedsDisplay:YES];
        return;
    }

    dragPoint = NSFarAwayPoint;

    if(highlightedControlPoint)
    {
        [NSCursor unhide];

        if(nextHighlightedControlPointSet)
        {
            nextHighlightedControlPointSet = NO;
            highlightedControlPoint = nextHighlightedControlPoint;
            highlightedSubpoint = nextHighlightedSubpoint;

            if(highlightedControlPoint == nil)
                [[NSCursor arrowCursor] set];
        }
    }

    [self setNeedsDisplay:YES];
}

- (void)mouseEntered:(NSEvent *)event
{
    if(!NSComparePoint(dragPoint, NSFarAwayPoint))
    {
        NSDictionary * userInfo = [event userData];
        nextHighlightedControlPoint = [userInfo objectForKey:@"point"];
        nextHighlightedSubpoint = [[userInfo objectForKey:@"subpoint"]
            intValue];
        nextHighlightedControlPointSet = YES;

        return;
    }

    NSDictionary * userInfo = [event userData];
    highlightedControlPoint = [userInfo objectForKey:@"point"];
    highlightedSubpoint = [[userInfo objectForKey:@"subpoint"] intValue];
    [self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event
{
    // Don't let the mouse out of the view's rect
    NSPoint mouse = [self convertPointFromBase:
        [[NSApp keyWindow] mouseLocationOutsideOfEventStream]];

    if(!NSPointInRect(mouse, [self bounds]))
        return;

    // If we're doing box select, don't update any control points
    if(!NSComparePoint(boxPoint, NSFarAwayPoint))
    {
        [self setNeedsDisplay:YES];
        return;
    }

    if(!highlightedControlPoint || [selection count] == 0 ||
       NSComparePoint(dragPoint, NSFarAwayPoint))
        return;

    BOOL snapAngle = ([event modifierFlags] & NSShiftKeyMask) != 0;

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

    if([selection count])
    {
        // Recalculate tracking areas for selected control points
        for(IPControlPointSelection * sel in selection)
        {
            NSArray * areas;
            areas = [controlPointSubareas objectForKey:sel.controlPoint];

            for(NSTrackingArea * ta in areas)
                [self removeTrackingArea:ta];

            [self createTrackingAreasForControlPoint:sel.controlPoint];
        }
    }

    [self setNeedsDisplay:YES];

    dragPoint = mouse;
}

- (void)mouseExited:(NSEvent *)event
{
    // If mouse is released from dragging, empty our highlight

    if(!NSComparePoint(dragPoint, NSFarAwayPoint))
    {
        nextHighlightedControlPoint = nil;
        nextHighlightedControlPointSet = YES;
        return;
    }

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
        NSArray * controlPoints = [[controlPoint parent] controlPoints];

        // Don't draw first and last control points, as they're not used
        /// \todo Also don't allow dragging them!!
        if((controlPoint == [controlPoints lastObject] && index == 1) ||
           (controlPoint == [controlPoints objectAtIndex:0] && index == 0))
            continue;

        if(sel && sel.subpoint == index)
        {
            // Subpoint is selected
            CGContextSetRGBStrokeColor(ctx, 0.788, 0.714, 0.110, 0.5);
            CGContextSetRGBFillColor(ctx, 0.988, 0.914, 0.310, 0.5);
            CGContextSetLineWidth(ctx, 1.5);
        }
        else if(controlPoint == highlightedControlPoint &&
                highlightedSubpoint == index)
        {
            // Subpoint is highlighted
            CGContextSetRGBStrokeColor(ctx, 0.247, 0.424, 0.612, 0.5);
            CGContextSetRGBFillColor(ctx, 0.447, 0.624, 0.812, 0.5);
            CGContextSetLineWidth(ctx, 1.5);
        }
        else
        {
            // Subpoint is neither selected nor highlighted
            CGContextSetRGBFillColor(ctx, 0.8, 0.8, 0.8, 0.5);
            CGContextSetRGBStrokeColor(ctx, 0.4, 0.4, 0.4, 0.5);
            CGContextSetLineWidth(ctx, 1.0);
        }

        if(index != 2)
        {
            // Draw small point for subpoints
            subpoint = [controlPoint absoluteControlPoint:index];
            ell = CGRectAroundPoint(subpoint, 2, 2);
        }
        else
        {
            // Draw large point for main control point
            subpoint = point;
            ell = CGRectAroundPoint(subpoint, 3, 3);
        }

        // Draw lines for handles
        if(index != 2)
        {
            CGContextMoveToPoint(ctx, point.x, point.y);
            CGContextAddLineToPoint(ctx, subpoint.x, subpoint.y);
            CGContextStrokePath(ctx);
        }

        if(index == 2)
            CGContextSetLineWidth(ctx, 1.5);

        if(index == 2 && [controlPoint type] == CU_CONTROL_POINT_CORNER)
        {
            // Draw *corner* main control points as squares
            CGContextAddRect(ctx, ell);
            CGContextFillPath(ctx);
            CGContextAddRect(ctx, ell);
            CGContextStrokePath(ctx);
        }
        else
        {
            CGContextAddEllipseInRect(ctx, ell);
            CGContextFillPath(ctx);
            CGContextAddEllipseInRect(ctx, ell);
            CGContextStrokePath(ctx);
        }

        if(index == 2)
            CGContextSetLineWidth(ctx, 1);
    }
}

// TODO: move this stuff into an IPCurve class (along with
// set of control points, etc.)
- (double)evaluateBezierParameterAtT:(double)t X1:(double)x1 X2:(double)x2
    X3:(double)x3 X4:(double)x4
{
    return ((pow(1.0 - t, 3.0) * x1) + (3.0 * pow(1.0 - t, 2.0) * t * x2) +
        (3.0 * (1.0 - t) * pow(t, 2.0) * x3) + (pow(t, 3.0) * x4));
}

- (NSPoint)evaluateBezierAtT:(double)t
    pointA:(IPControlPoint *)a
    pointB:(IPControlPoint *)b
{
    NSPoint p0, p1, p2, p3;

    p0 = [a point];
    p1 = [a absoluteControlPoint:1];
    p2 = [b absoluteControlPoint:0];
    p3 = [b point];

    return NSMakePoint(
        [self evaluateBezierParameterAtT:t X1:p0.x X2:p1.x X3:p2.x X4:p3.x],
        [self evaluateBezierParameterAtT:t X1:p0.y X2:p1.y X3:p2.y X4:p3.y]);
}

NSInteger controlPointSort(id point1, id point2, void * ctx)
{
    IPControlPoint * a = (IPControlPoint *)point1;
    IPControlPoint * b = (IPControlPoint *)point2;

    return [a point].x > [b point].x ? 1 : -1;
}

- (void)drawCurve:(IPCurve *)curve
{
    IPControlPoint * a, * b;
    NSArray * controlPoints = [curve controlPoints];
    BOOL validPath = YES;
    CGFloat dashPhase[] = {3, 6};
    NSPoint lastPoint;
    IPColor color;

    controlPoints = [controlPoints sortedArrayUsingFunction:controlPointSort
        context:nil];
    [curve setControlPoints:controlPoints];

    color = [[IPColors colors] colorAtIndex:[curves indexOfObject:curve]];

    CGContextRef ctx = [[NSGraphicsContext currentContext] graphicsPort];
    CGContextSetInterpolationQuality(ctx, kCGInterpolationHigh);
    CGContextSetRGBStrokeColor(ctx, color.r, color.g, color.b, color.a);
    CGContextSetLineWidth(ctx, 1.0);

    for(unsigned int i = 0; i < [controlPoints count] - 1; i++)
    {
        a = [controlPoints objectAtIndex:i];
        b = [controlPoints objectAtIndex:i+1];

        CGContextMoveToPoint(ctx, [a point].x, [a point].y);

        for(double t = 0.0; t < 1.0; t += 0.001) /// \todo Adaptive timestep?
        {
            NSPoint bzpt = [self evaluateBezierAtT:t pointA:a pointB:b];
            CGContextAddLineToPoint(ctx, bzpt.x, bzpt.y);

            if(t != 0.0 && lastPoint.x > bzpt.x)
                validPath = NO;

            lastPoint = bzpt;
        }
    }

    if(!validPath)
        CGContextSetLineDash(ctx, 0, dashPhase, 2);

    CGContextStrokePath(ctx);

    CGContextSetLineDash(ctx, 0, NULL, 0);
}

- (void)drawRect:(NSRect)dirtyRect
{
    [[NSColor whiteColor] setFill];
    NSRectFill(dirtyRect);

    for(IPCurve * curve in curves)
        [self drawCurve:curve];

    if(drawControlPoints) // TODO: disable manipulation too
        for(IPCurve * curve in curves)
            for(IPControlPoint * controlPoint in [curve controlPoints])
                [self drawPoint:controlPoint];

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

- (BOOL)drawControlPoints
{
    return drawControlPoints;
}

- (void)setDrawControlPoints:(BOOL)newDrawControlPoints
{
    drawControlPoints = newDrawControlPoints;
    [self setNeedsDisplay:YES];
}

- (void)setCurveSet:(IPCurveSet *)newCurves
{
    if(displayCurveSet != newCurves)
        [self clearSelection];

    displayCurveSet = newCurves;
    [self updateCurves];
}

- (void)clearSelection
{
    if(selection == nil)
        selection = [[NSMutableArray alloc] init];
    else
        [selection removeAllObjects];

    [self updateSelection];
}

- (void)updateSelection
{
    IPControlPointSelection * sel = [selection lastObject];

    // Update control point type button state. Disable it if selection is
    // comprised of handles or empty, update the selected state otherwise.
    if([selection count] == 0 || sel.subpoint != 2)
    {
        [delegate disableControlPointTypeButtons];
        return;
    }

    [delegate enableControlPointTypeButtons];
    /// \todo if multiple selection, don't press any buttons (or something)?
    [delegate updateControlPointType:sel.controlPoint.type];
}

- (void)setSelectedControlPointType:(int)type
{
    IPControlPointSelection * sel = [selection lastObject];

    // This should never happen, because in this case, the button should always
    // be disabled.
    if([selection count] == 0 || sel.subpoint != 2)
        return;

    for(IPControlPointSelection * sel in selection)
    {
        [sel.controlPoint setType:type];
    }

    [self updateCurves];
}

@end
