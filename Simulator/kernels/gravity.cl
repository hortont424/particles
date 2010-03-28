/* particles - simulator - gravity.cl
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

#include "libparticles.h"

#define GRAV_CONSTANT 0.0000000000667300f

__kernel void gravity(__global PAPhysicsParticle * input,
                      __global PAPhysicsParticle * output,
                      __global PAPhysicsNewtonian * newtonIn,
                      __global PAPhysicsNewtonian * newtonOut,
                      const unsigned int count)
{
    float dist;
    float4 loc, accel, vel, iloc, dir;
    loc = accel = vel = iloc = dir = (float4)(0.0f);

    int id = get_global_id(0);

    if(id > count || input[id].enabled == 0.0)
        return;

    loc = (float4)(input[id].x, input[id].y, input[id].z, 0.0f);

    for(int i = 0; i < count; i++)
    {
        if(i == id)
            continue;

        iloc = (float4)(input[i].x, input[i].y, input[i].z, 0.0f);
        dir = normalize(iloc - loc);
        dist = distance(loc, iloc);

        if(dist < 1.0)
            dist = 1.0;

        accel += dir * ((GRAV_CONSTANT * newtonIn[i].mass) / pow(dist, 2));
    }

    newtonOut[id].ax = accel.x;
    newtonOut[id].ay = accel.y;
    newtonOut[id].az = accel.z;
}
