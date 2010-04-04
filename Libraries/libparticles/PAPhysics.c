/* particles - libparticles - PAPhysics.c
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "PAPhysics.h"

#define readJSONProperty(src, dest, prop, def) \
{ \
    json_object * tmp; \
    tmp = json_object_object_get(src, #prop); \
    dest.prop = (tmp != NULL ? json_object_get_double(tmp) : def); \
}

PAPhysicsForce * PAPhysicsForceNew(PAPhysicsType type)
{
    PAPhysicsForce * force;

    force = (PAPhysicsForce *)calloc(1, sizeof(PAPhysicsForce));
    force->type = type;

    return force;
}

PAPhysicsForce * PAPhysicsForceNewFromJSON(json_object * js)
{
    const char * typestr;
    PAPhysicsForce * force;
    json_object * particle;

    particle = json_object_object_get(js, "particle");
    typestr = json_object_get_string(json_object_object_get(js, "type"));

    /// \todo Implement the rest of the PAPhysicsTypes here
    if(strcmp(typestr, "gravity") == 0)
    {
        force = PAPhysicsForceNew(PAPhysicsGravityType);

        readJSONProperty(js, force->forceData.gravity, strength, 1.0);
        readJSONProperty(js, force->forceData.gravity, noise, 1.0);
    }

    readJSONProperty(particle, force->particle, enabled, 1.0);
    readJSONProperty(particle, force->particle, x, 0.0);
    readJSONProperty(particle, force->particle, y, 0.0);
    readJSONProperty(particle, force->particle, z, 0.0);

    return force;
}
