/* particles - libparticles - PASystem.c
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
#include <stdlib.h>
#include <stdio.h>

#include <liblog/liblog.h>

#include "PASystem.h"
#include "PAError.h"

PASystem * PASystemNew()
{
    PASystem * sys;

    sys = (PASystem *)calloc(1, sizeof(PASystem));

    return sys;
}

PASystem * PASystemNewFromJSON(json_object * jsSystem)
{
    PASystem * sys;
    json_object * jsForces, * jsInitialParticles, *jsPartCount;
    array_list * forces;

    sys = PASystemNew();

    if(!(jsForces = json_object_object_get(jsSystem, "forces")))
    {
        malformedFileError("forces");
        PASystemFree(sys);
        return NULL;
    }

    if(!(forces = json_object_get_array(jsForces)))
    {
        malformedFileError("forces");
        PASystemFree(sys);
        return NULL;
    }

    if(!(jsInitialParticles = json_object_object_get(jsSystem,
                                                     "initialParticles")))
    {
        malformedFileError("initialParticles");
        PASystemFree(sys);
        return NULL;
    }

    if(!(jsPartCount = json_object_object_get(jsInitialParticles, "count")))
    {
        malformedFileError("initialParticles->count");
        PASystemFree(sys);
        return NULL;
    }

    sys->initialParticleCount = json_object_get_double(jsPartCount);
    sys->forceCount = array_list_length(forces);
    sys->forces = (PAPhysicsForce **)calloc(sys->forceCount,
                                            sizeof(PAPhysicsForce *));

    for(int i = 0; i < sys->forceCount; i++)
    {
        json_object * jsForce;

        if(!(jsForce = (json_object *)array_list_get_idx(forces, i)))
        {
            malformedFileError("force");
            PASystemFree(sys);
            return NULL;
        }

        sys->forces[i] = PAPhysicsForceNewFromJSON(jsForce);
    }

    return sys;
}

PASystem * PASystemNewFromFile(const char * filename)
{
    json_object * jsSystem;

     // why not const

    if(!(jsSystem = json_object_from_file((char *)filename)))
    {
        LOError("psys file '%s' not found", filename);
        return NULL;
    }

    return PASystemNewFromJSON(jsSystem);
}

void PASystemFree(PASystem * sys)
{
    if(!sys)
        return;

    if(sys->forces)
    {
        for(unsigned int i = 0; i < sys->forceCount; i++)
            if(sys->forces[i])
                free(sys->forces[i]);

        free(sys->forces);
    }

    free(sys);
}
