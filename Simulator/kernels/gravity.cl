#include "SMPhysics.h"

__kernel void gravity(__global SMPhysicsParticle * input,
                      __global SMPhysicsNewtonian * newton,
                      __global SMPhysicsParticle * output,
                      const unsigned int count)
{
    float4 loc, accel, vel, iloc, dir;
    float grav = 0.0000000000667300f;
    float dist;
    int id = get_global_id(0);

    if(id > count)
        return;

    loc = (float4)(input[id].x, input[id].y, input[id].z, 0.0f);
    vel = (float4)(newton[id].vx, newton[id].vy, newton[id].vz, 0.0f);
    accel = (float4)(0.0f);
    int n = 0;
    for(int i = 0; i < count; i++)
    {
        if(i == id)
            continue;

        iloc = (float4)(input[i].x, input[i].y, input[i].z, 0.0f);
        dir = normalize(iloc - loc);
        dist = distance(loc, iloc);
        if(dist < 2.0)
            dist = 2.0;
        accel += dir * ((grav * newton[i].mass) / pow(dist, 2));

        n = i;
    }

    vel += (accel * 0.01);
    //vel *= 0.95;

    loc += vel * 0.01;

    output[id].x = loc.x;
    output[id].y = loc.y;
    output[id].z = loc.z;

    newton[id].vx = vel.x;
    newton[id].vy = vel.y;
    newton[id].vz = vel.z;
}
