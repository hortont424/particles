#include "SMPhysics.h"

__kernel void euler_integration(__global SMPhysicsParticle * input,
                                __global SMPhysicsParticle * output,
                                __global SMPhysicsNewtonian * newtonIn,
                                __global SMPhysicsNewtonian * newtonOut,
                                const unsigned int count)
{
    int id = get_global_id(0);

    if(id > count || input[id].enabled == 0.0)
        return;

    // Integrate velocity
    newtonOut[id].vx = newtonIn[id].vx + (newtonOut[id].ax * 0.005);
    newtonOut[id].vy = newtonIn[id].vy + (newtonOut[id].ay * 0.005);
    newtonOut[id].vz = newtonIn[id].vz + (newtonOut[id].az * 0.005);

    // Integrate position
    output[id].x = input[id].x + (newtonOut[id].vx * 0.005);
    output[id].y = input[id].y + (newtonOut[id].vy * 0.005);
    output[id].z = input[id].z + (newtonOut[id].vz * 0.005);

    // Clear initial acceleration for next frame
    newtonOut[id].ax = 0.0;
    newtonOut[id].ay = 0.0;
    newtonOut[id].az = 0.0;

    // Pass through useful properties
    newtonOut[id].mass = newtonIn[id].mass;
    output[id].enabled = input[id].enabled;
}
