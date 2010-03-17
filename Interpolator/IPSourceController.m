/* particles - interpolator - IPSourceController.m
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

#import "IPSourceController.h"

@implementation IPSourceController

- (void)outlineViewSelectionDidChange:(NSNotification *)aNotification
{
    id item = [curveSetChooser itemAtRow:[curveSetChooser selectedRow]];

    if([item isMemberOfClass:[IPCurve class]])
        item = [curveSetChooser parentForItem:item];

    [controlPointView setCurvesIndex:[curveSetChooser rowForItem:item]];
}

- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object
    forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
    IPCurveSet * set = item;
    [set setName:(NSString *)object];
}

- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index
    ofItem:(id)item
{
    if(item == nil)
    {
        return [[curveStorage curveSets] objectAtIndex:index];
    }
    else
    {
        if([item isMemberOfClass:[IPCurveSet class]])
            return [[item curves] objectAtIndex:index];
    }

    return nil;
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView
    numberOfChildrenOfItem:(id)item
{
    if(item == nil)
    {
        return [[curveStorage curveSets] count];
    }
    else
    {
        if([item isMemberOfClass:[IPCurveSet class]])
            return [[item curves] count];
    }

    return 0;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
    return [item isMemberOfClass:[IPCurveSet class]];
}

- (id)outlineView:(NSOutlineView *)outlineView
    objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
    return [item name];
}

@end
