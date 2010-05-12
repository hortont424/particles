/* particles - simulator - boids.cl
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

__kernel void boids(__global PAPhysicsParticle * input,
                    __global PAPhysicsParticle * output,
                    __global PAPhysicsNewtonian * newtonIn,
                    __global PAPhysicsNewtonian * newtonOut,
                    __global PAPhysicsForce * force,
                    const unsigned int count)
{
    int id = get_global_id(0);
    float4 iloc, bPos, accel, bVel;
    accel = (float4)(0.0f);

    if(id > count || input[id].lifetime == 0)
        return;

    bPos = (float4)(input[id].x, input[id].y, input[id].z, 0.0f);
    bVel = (float4)(newtonIn[id].ox, newtonIn[id].oy, newtonIn[id].oz, 0.0f);

    // Move towards COG
    float4 centerOfGravity = (float4)0.0f;

    for(int i = 0; i < count; i++)
    {
        if(i == id)
            continue;

        iloc = (float4)(input[i].x, input[i].y, input[i].z, 0.0f);

        centerOfGravity += iloc;
    }

    centerOfGravity /= count - 1.0f;
    accel += (centerOfGravity - bPos) / 100.0f;

    // Avoid each other
    float4 avoidAmount = (float4)0.0f;

    for(int i = 0; i < count; i++)
    {
        if(i == id)
            continue;

        iloc = (float4)(input[i].x, input[i].y, input[i].z, 0.0f);

        if(distance(iloc, bPos) < 0.1)
        {
            avoidAmount -= (iloc - bPos);
        }
    }

    accel += avoidAmount;

    // Match velocities
    float4 matchVelocity = (float4)0.0f;

    for(int i = 0; i < count; i++)
    {
        if(i == id)
            continue;

        // actually ivel
        iloc = (float4)(newtonIn[i].ox, newtonIn[i].oy, newtonIn[i].oz, 0.0f);

        matchVelocity += iloc;
    }

    matchVelocity /= count - 1.0f;
    accel += (matchVelocity - bVel) / 8.0f;

    newtonIn[id].ox = input[id].x - accel.x / 100.0;
    newtonIn[id].oy = input[id].y - accel.y / 100.0;
    newtonIn[id].oz = input[id].z - accel.z / 100.0;
}