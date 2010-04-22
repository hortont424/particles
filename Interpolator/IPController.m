/* particles - interpolator - IPController.m
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

#import "IPController.h"
#import "IPCurveSet.h"

@implementation IPController

- (void)awakeFromNib
{
    [curveStorage load];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
    [curveStorage save];
}

- (IBAction)setControlPointType:(id)sender
{
    [controlPointView setSelectedControlPointType:
        [controlPointTypeButtons selectedSegment]];
}

- (IBAction)setDrawControlPoints:(id)sender
{
    [controlPointView setDrawControlPoints:![sender state]];
}

- (IBAction)addCurveSet:(id)sender
{
    [curveStorage addCurveSet];
    [self updateCurveSets];
}

- (IBAction)removeCurveSet:(id)sender
{
    NSLog(@"remove curve set");
}

- (IBAction)addCurve:(id)sender
{
    // Find selected curve set (if a curve is selected, take its parent)
    id item = [curveSetChooser itemAtRow:[curveSetChooser selectedRow]];

    if([item isMemberOfClass:[IPCurve class]])
        item = [curveSetChooser parentForItem:item];

    IPControlPoint * pt;
    IPCurve * curve = [[IPCurve alloc] init];

    pt = [[IPControlPoint alloc] init];
    [pt setPoint:NSMakePoint(20, 20)];
    [pt setType:CU_CONTROL_POINT_CORNER];
    [pt setControlPoint:0 toPoint:NSMakePoint(-2, -12)];
    [pt setControlPoint:1 toPoint:NSMakePoint(15, 15)];
    [curve addControlPoint:pt];

    pt = [[IPControlPoint alloc] init];
    [pt setPoint:NSMakePoint(120, 120)];
    [pt setType:CU_CONTROL_POINT_CORNER];
    [pt setControlPoint:0 toPoint:NSMakePoint(-2, -12)];
    [pt setControlPoint:1 toPoint:NSMakePoint(15, 15)];
    [curve addControlPoint:pt];

    [item addCurve:curve];
    [self updateCurveSets];
    [controlPointView updateCurves];
}

- (void)updateCurveSets
{
    [curveSetChooser reloadData];
}

- (void)enableControlPointTypeButtons
{
    [controlPointTypeButtons setEnabled:YES];
    [[controlPointTypeButtons cell] setTrackingMode:
        NSSegmentSwitchTrackingSelectOne];
}

- (void)disableControlPointTypeButtons
{
    [controlPointTypeButtons setEnabled:NO];
    [[controlPointTypeButtons cell] setTrackingMode:
        NSSegmentSwitchTrackingMomentary];
}

- (void)updateControlPointType:(int)type
{
    [controlPointTypeButtons setSelectedSegment:type];
}

@end
