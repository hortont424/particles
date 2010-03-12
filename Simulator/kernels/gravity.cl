#include "SMPhysics.h"

#define GRAV_CONSTANT 0.0000000000667300f

__kernel void gravity(__global SMPhysicsParticle * input,
                      __global SMPhysicsParticle * output,
                      __global SMPhysicsNewtonian * newtonIn,
                      __global SMPhysicsNewtonian * newtonOut,
                      const unsigned int count)
{
    float4 loc, accel, vel, iloc, dir;

    float dist;
    int id = get_global_id(0);

    if(id > count || input[id].enabled == 0.0)
        return;

    accel = (float4)(0.0f);
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
