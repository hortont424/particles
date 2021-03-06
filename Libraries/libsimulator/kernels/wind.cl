/* particles - simulator - wind.cl
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

__kernel void wind(__global PAPhysicsParticle * input,
                   __global PAPhysicsParticle * output,
                   __global PAPhysicsNewtonian * newtonIn,
                   __global PAPhysicsNewtonian * newtonOut,
                   __global PAPhysicsForce * force,
                   const unsigned int count)
{
    int id = get_global_id(0);
    float4 fpoint, loc, accel;
    float dist;
    fpoint = loc = accel = (float4)(0.0f);

    if(id > count || input[id].lifetime == 0)
        return;

    loc = (float4)(input[id].x, input[id].y, input[id].z, 0.0f);
    fpoint = (float4)(force->particle.x, force->particle.y,
                      force->particle.z, 0.0f);

    accel = force->data.wind.strength * fpoint;
    dist = distance(loc, fpoint);
    accel = accel * (1.0f / powr(dist + 1.0f,
                                 force->data.wind.falloff.strength));

    if(dist < force->data.wind.falloff.min ||
       dist > force->data.wind.falloff.max)
        accel = (float4)(0.0f);

    newtonOut[id].ax += accel.x;
    newtonOut[id].ay += accel.y;
    newtonOut[id].az += accel.z;
}