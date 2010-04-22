/* particles - interpolator - IPCurve.m
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

#import "IPCurve.h"

@implementation IPCurve

@synthesize name;

- (id)init
{
    self = [super init];

    if(self)
    {
        //IPControlPoint * pt;
        controlPoints = [[NSMutableArray alloc] init];
        name = @"x";
    }
    return self;
}

// Accessors

- (NSArray *)controlPoints
{
    return controlPoints;
}

// Mutators

- (void)addControlPoint:(IPControlPoint *)controlPoint
{
    [controlPoints addObject:controlPoint];
    [controlPoint setParent:self];
}

- (void)removeControlPoint:(IPControlPoint *)controlPoint
{
    [controlPoints removeObject:controlPoint];
    [controlPoint setParent:nil];
}

- (void)setControlPoints:(NSArray *)cps
{
    controlPoints = [cps mutableCopy];

    for(IPControlPoint * cp in controlPoints)
    {
        [cp setParent:self];
    }
}

// (De)Coding

- (id)initWithCoder:(NSCoder *)coder
{
    if((self = [super init]))
    {
        controlPoints = [coder decodeObjectForKey:@"controlPoints"];

        for(IPControlPoint * cp in controlPoints)
        {
            [cp setParent:self];
        }

        name = [coder decodeObjectForKey:@"name"];
    }

    return self;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
    [coder encodeObject:controlPoints forKey:@"controlPoints"];
    [coder encodeObject:name forKey:@"name"];
}

@end
