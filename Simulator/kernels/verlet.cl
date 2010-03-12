#include "SMPhysics.h"

__kernel void verlet(__global SMPhysicsParticle * input,
                     __global SMPhysicsParticle * output,
                     __global SMPhysicsNewtonian * newtonIn,
                     __global SMPhysicsNewtonian * newtonOut,
                     const unsigned int count)
{
    int id = get_global_id(0);

    if(id > count || input[id].enabled == 0.0)
        return;

    // Extract useful vectors; set the next frame's old position to our position
    float4 acceleration, position, oldPosition;
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
}
