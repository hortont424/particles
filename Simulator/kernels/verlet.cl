/* particles - simulator - verlet.cl
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

__kernel void verlet(__global PAPhysicsParticle * input,
                     __global PAPhysicsParticle * output,
                     __global PAPhysicsNewtonian * newtonIn,
                     __global PAPhysicsNewtonian * newtonOut,
                     const unsigned int count)
{
    int id = get_global_id(0);

    if(id > count || input[id].enabled == 0.0)
        return;

    // Extract useful vectors; set the next frame's old position to our position
    float4 acceleration, position, oldPosition;
    acceleration = position = oldPosition = (float4)(0.0f);

    acceleration.x = newtonOut[id].ax;
    acceleration.y = newtonOut[id].ay;
    acceleration.z = newtonOut[id].az;
    newtonOut[id].ox = position.x = input[id].x;
    newtonOut[id].oy = position.y = input[id].y;
    newtonOut[id].oz = position.z = input[id].z;
    oldPosition.x = newtonIn[id].ox;
    oldPosition.y = newtonIn[id].oy;
    oldPosition.z = newtonIn[id].oz;

    // Perform Verlet integration
    position += (position - oldPosition) + (acceleration * 0.005 * 0.005);

    output[id].x = position.x;
    output[id].y = position.y;
    output[id].z = position.z;

    // Pass through useful properties
    newtonOut[id].mass = newtonIn[id].mass;
    output[id].enabled = input[id].enabled;

    // Clear acceleration for next simulation step
    newtonIn[id].ax = newtonIn[id].ay = newtonIn[id].az = 0.0;
}
