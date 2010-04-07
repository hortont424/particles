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

#include <liblog/liblog.h>

#include "PAPhysics.h"
#include "PAError.h"

#define readJSONProperty(src, dest, prop, def) \
{ \
    json_object * tmp; \
    if(!(tmp = json_object_object_get(src, #prop))) \
    { \
        malformedFileError(#src "->" #prop); \
    } \
    else \
    { \
        dest.prop = (tmp != NULL ? json_object_get_double(tmp) : def); \
    } \
}

PAPhysicsForce * PAPhysicsForceNew(PAPhysicsType type)
{
    PAPhysicsForce * force;

    force = (PAPhysicsForce *)calloc(1, sizeof(PAPhysicsForce));
    force->type = type;

    return force;
}

PAPhysicsForce * PAPhysicsForceNewFromJSON(json_object * jsForce)
{
    const char * typestr = NULL;
    PAPhysicsForce * force = NULL;
    json_object * jsParticle = NULL, * jsKernel = NULL;

    if(!(jsParticle = json_object_object_get(jsForce, "particle")))
    {
        malformedFileError("force->particle");
        return NULL;
    }

    if(!(jsKernel = json_object_object_get(jsForce, "kernel")))
    {
        malformedFileError("force->kernel");
        return NULL;
    }

    if(!(typestr = json_object_get_string(jsKernel)))
    {
        malformedFileError("force->kernel");
        return NULL;
    }

    /// \todo Implement the rest of the PAPhysicsTypes here
    if(strcmp(typestr, "gravity") == 0)
    {
        force = PAPhysicsForceNew(PAPhysicsGravityType);

        readJSONProperty(jsForce, force->forceData.gravity, strength, 1.0);
        readJSONProperty(jsForce, force->forceData.gravity, noise, 1.0);
    }
    else
    {
        printf("Error: unknown kernel %s in psys file!\n", typestr);
        return NULL;
    }

    readJSONProperty(jsParticle, force->particle, enabled, 1.0);
    readJSONProperty(jsParticle, force->particle, x, 0.0);
    readJSONProperty(jsParticle, force->particle, y, 0.0);
    readJSONProperty(jsParticle, force->particle, z, 0.0);

    return force;
}
