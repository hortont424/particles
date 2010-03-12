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

    // Compute acceleration
    float4 accel;
    accel.x = newtonOut[id].ax;
    accel.y = newtonOut[id].ay;
    accel.z = newtonOut[id].az;

    // Perform Verlet integration
    float4 position, oldPosition;
    newtonOut[id].ox = position.x = input[id].x;
    newtonOut[id].oy = position.y = input[id].y;
    newtonOut[id].oz = position.z = input[id].z;
    oldPosition.x = newtonIn[id].ox;
    oldPosition.y = newtonIn[id].oy;
    oldPosition.z = newtonIn[id].oz;

    position += (position - oldPosition) + (accel * 0.005 * 0.005);

    output[id].x = position.x;
    output[id].y = position.y;
    output[id].z = position.z;

    // Pass through useful properties
    newtonOut[id].mass = newtonIn[id].mass;
    output[id].enabled = input[id].enabled;
}
