/* particles - simulator - render.cl
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

#include "libparticles/PATypes.h"

__kernel void render(__global PAPhysicsParticle * input,
                     __global PAPhysicsNewtonian * newton,
                     __global PAFloat * output,
                     int count,
                     int resolution)
{
    int id = get_global_id(0);
    float4 iloc, loc, realLoc;
    float dist;

    loc = (float4)((float)(id % resolution), (float)id / (float)resolution, 0.0f, 0.0f);
    realLoc = loc / ((float)resolution);

    float scale = 5.0;
    realLoc *= scale;
    realLoc.x += (0.5 - (scale / 2.0f));
    realLoc.y += (0.5 - (scale / 2.0f));

    output[id] = 0;

    float colorVal = 0.0;

    for(int i = 0; i < count; i++)
    {
        iloc = (float4)(input[i].x, input[i].y, 0.0f/*input[i].z*/, 0.0f);
        dist = distance(realLoc, iloc);

        if(dist < 0.1)
            colorVal += 1.0f;//(0.1 - dist) * 50;
    }

    if(colorVal > 255)
        output[id] = 255;
    else
        output[id] = colorVal;
}
