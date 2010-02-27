/*
    0 1 2 3 4 5 6
    x y z m vxvyvz
*/

__kernel void gravity(__global float * input, __global float * output,
                      const unsigned int count)
{
    float4 loc, accel, vel, iloc;
    float grav = 0.0000000000667300f;
    int id = get_global_id(0);
    
    if(id > count)
        return;
    
    id *= 7;
    
    loc = (float4)(input[id + 0], input[id + 0], input[id + 0], 0.0f);
    vel = (float4)(input[id + 4], input[id + 5], input[id + 6], 0.0f);
    accel = (float4)(0.0f);
    
    for(int i = 0; i < count * 7; i += 7)
    {
        if(i == id)
            continue;
        
        iloc = (float4)(input[i + 0], input[i + 0], input[i + 0], 0.0f);
        accel += (grav * input[i + 3]) / distance(loc, iloc);
    }
    
    vel += (accel * 0.1);
    loc += vel * 0.1;
    
    output[id + 0] = loc.x;
    output[id + 1] = loc.y;
    output[id + 2] = loc.z;
    
    output[id + 4] = vel.x;
    output[id + 5] = vel.y;
    output[id + 6] = vel.z;
}
