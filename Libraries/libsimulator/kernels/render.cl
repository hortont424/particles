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
    float4 iloc, loc, realLoc, position, oldPosition;
    float dist;

    position = oldPosition = (float4)(0.0f);

    if(id >= (resolution * resolution))
        return;

    loc = (float4)((float)(id % resolution), (float)id / (float)resolution, 0.0f, 0.0f);
    realLoc = loc / ((float)resolution);

    float scale = 4.0;
    realLoc *= scale;
    realLoc.x += (0.5 - (scale / 2.0f));
    realLoc.y += (0.5 - (scale / 2.0f));

    output[(id * 4) + 0] = 0;
    output[(id * 4) + 1] = 0;
    output[(id * 4) + 2] = 0;
    output[(id * 4) + 3] = 255;

    float mag = 0.0;

    for(int i = 0; i < count; i++)
    {
        if(input[i].lifetime == 0)
            continue;

        iloc = (float4)(input[i].x, input[i].y, 0.0f, 0.0f);
        dist = distance(realLoc, iloc);

        position.x = input[i].x;
        position.y = input[i].y;
        position.z = input[i].z;
        oldPosition.x = newton[i].ox;
        oldPosition.y = newton[i].oy;
        oldPosition.z = newton[i].oz;

        float veloc = distance(position, oldPosition) * 8.0;

        if(dist < 0.005)
        {
            mag = (0.005 - dist) * 200000.0f;

            output[(id * 4) + 0] += mag;
            output[(id * 4) + 1] += fabs(0.5f + input[i].z) * 50.0f;
            output[(id * 4) + 2] += veloc;
        }
    }

    for(int i = 0; i < 4; i++)
        output[(id * 4) + i] = fmin(output[(id * 4) + i], 255.0f);
}
