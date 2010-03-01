/*
    0 1 2 3 4 5 6
    x y z m vxvyvz
*/

__kernel void gravity(__global float * input,
                      __global float * output,
                      const unsigned int count)
{
    float4 loc, accel, vel, iloc, dir;
    float grav = 0.0000000000667300f;
    float dist;
    int id = get_global_id(0);
    
    if(id > count)
        return;
    
    id *= 7;
    
    loc = (float4)(input[id + 0], input[id + 1], input[id + 2], 0.0f);
    vel = (float4)(input[id + 4], input[id + 5], input[id + 6], 0.0f);
    accel = (float4)(0.0f);
    int n = 0;
    for(int i = 0; i < count * 7; i += 7)
    {
        if(i == id)
            continue;
        
        iloc = (float4)(input[i + 0], input[i + 1], input[i + 2], 0.0f);
        dir = normalize(iloc - loc);
        dist = distance(loc, iloc);
        if(dist < 2.0)
            dist = 2.0;
        accel += dir * ((grav * input[i + 3]) / pow(dist, 2));
        
        n = i;
    }
    
    vel += (accel * 0.01);
    //vel *= 0.95;
    
    loc += vel * 0.01;
    
    output[id + 0] = loc.x;
    output[id + 1] = loc.y;
    output[id + 2] = loc.z;
    
    output[id + 4] = vel.x;
    output[id + 5] = vel.y;
    output[id + 6] = vel.z;
}
