#include "SMPhysics.h"

__kernel void passthrough(__global SMPhysicsParticle * input,
                          __global SMPhysicsNewtonian * newton,
                          __global SMPhysicsParticle * output,
                          const unsigned int count)
{
    int id = get_global_id(0);

    if(id > count)
        return;

    output[id].enabled = input[id].enabled;
    output[id].x = input[id].x;
    output[id].y = input[id].y;
    output[id].z = input[id].z;
}
