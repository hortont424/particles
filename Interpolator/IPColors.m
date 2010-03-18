/* particles - interpolator - IPColors.m
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

#import "IPColors.h"

@implementation IPColors

static IPColors * sharedColors = nil;

+ (IPColors *)colors
{
    if(sharedColors == nil)
    {
        sharedColors = [[super allocWithZone:NULL] init];
        [sharedColors loadColors];
    }

    return sharedColors;
}

+ (id)allocWithZone:(NSZone *)zone
{
    return [self colors];
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (void)loadColors
{
    int n = 0;

    colorCount = 21;
    colors = (IPColor *)calloc(colorCount, sizeof(IPColor));

    // Tango Colors: http://tango.freedesktop.org/Tango_Icon_Theme_Guidelines

    IPColor scarlet1 = {0.937, 0.161, 0.161, 1.0};
    IPColor scarlet2 = {0.800, 0.000, 0.000, 1.0};
    IPColor scarlet3 = {0.643, 0.000, 0.000, 1.0};

    IPColor sky1 = {0.447, 0.624, 0.812, 1.0};
    IPColor sky2 = {0.204, 0.396, 0.643, 1.0};
    IPColor sky3 = {0.122, 0.282, 0.514, 1.0};

    IPColor chameleon1 = {0.541, 0.886, 0.204, 1.0};
    IPColor chameleon2 = {0.451, 0.824, 0.086, 1.0};
    IPColor chameleon3 = {0.306, 0.604, 0.024, 1.0};

    IPColor orange1 = {0.988, 0.686, 0.243, 1.0};
    IPColor orange2 = {0.961, 0.475, 0.000, 1.0};
    IPColor orange3 = {0.808, 0.361, 0.000, 1.0};

    IPColor plumb1 = {0.678, 0.498, 0.659, 1.0};
    IPColor plumb2 = {0.459, 0.314, 0.482, 1.0};
    IPColor plumb3 = {0.361, 0.208, 0.400, 1.0};

    IPColor butter1 = {0.988, 0.914, 0.310, 1.0};
    IPColor butter2 = {0.929, 0.831, 0.000, 1.0};
    IPColor butter3 = {0.769, 0.627, 0.000, 1.0};

    IPColor chocolate1 = {0.914, 0.725, 0.431, 1.0};
    IPColor chocolate2 = {0.757, 0.490, 0.067, 1.0};
    IPColor chocolate3 = {0.561, 0.349, 0.008, 1.0};

    colors[n++] = scarlet1;
    colors[n++] = sky1;
    colors[n++] = chameleon1;
    colors[n++] = orange1;
    colors[n++] = plumb1;
    colors[n++] = butter1;
    colors[n++] = chocolate1;

    colors[n++] = scarlet3;
    colors[n++] = sky3;
    colors[n++] = chameleon3;
    colors[n++] = orange3;
    colors[n++] = plumb3;
    colors[n++] = butter3;
    colors[n++] = chocolate3;

    colors[n++] = scarlet2;
    colors[n++] = sky2;
    colors[n++] = chameleon2;
    colors[n++] = orange2;
    colors[n++] = plumb2;
    colors[n++] = butter2;
    colors[n++] = chocolate2;
}

// Accessors

- (IPColor)colorAtIndex:(unsigned int)index
{
    return colors[index % colorCount];
}

@end
