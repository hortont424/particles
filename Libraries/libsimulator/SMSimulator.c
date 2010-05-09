/* particles - libsimulator - SMSimulator.c
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

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <liblog/liblog.h>

#include "libsimulator.h"

SMSimulator * SMSimulatorNew(unsigned long elementCount)
{
    SMSimulator * sim;

    sim = (SMSimulator *)calloc(1, sizeof(SMSimulator));
    sim->elementCount = elementCount;
    sim->computer = COContextNew();
    sim->library = SMProgramLibraryNew(sim->computer);

    SMProgramLibrarySetGlobalCount(sim->library, elementCount);

    // Allocate native buffers for particles / newtonian properties
    sim->particles = (PAPhysicsParticle *)
        calloc(elementCount, sizeof(PAPhysicsParticle));

    sim->newtonian = (PAPhysicsNewtonian *)
        calloc(elementCount, sizeof(PAPhysicsNewtonian));

    if(!sim->particles || !sim->newtonian)
    {
        LOError("out of native memory");
        SMProgramLibraryFree(sim->library);
        COContextFree(sim->computer);

        if(sim->particles)
            free(sim->particles);

        if(sim->newtonian)
            free(sim->newtonian);

        free(sim);
    }

    // Allocate remote (OpenCL) buffers to copy native buffers to/from
    sim->clParticles = COBufferNew(sim->computer, elementCount,
                                   sizeof(PAPhysicsParticle), true);
    sim->clNewtonian = COBufferNew(sim->computer, elementCount,
                                   sizeof(PAPhysicsNewtonian), true);

    LOLog("Allocated video memory (%ld KB)",
          ((2 * elementCount * sizeof(PAPhysicsParticle)) +
          (2 * elementCount * sizeof(PAPhysicsNewtonian))) / 1024);

    return sim;
}

SMSimulator * SMSimulatorNewFromFile(const char * filename)
{
    SMSimulator * sim;
    PASystem * sys;

    sys = PASystemNewFromFile(filename);

    sim = SMSimulatorNew(sys->initialParticleCount);
    sim->system = sys;

    sim->forcePrograms = (COProgram **)calloc(sim->system->forceCount,
                                              sizeof(COProgram *));

    // Load each force from the PASystem; instantiate a copy of the necessary
    // kernel, setting various arguments as needed
    for(unsigned int i = 0; i < sim->system->forceCount; i++)
    {
        PAPhysicsForce * force;
        COProgram * k;
        COBuffer * forceBuf;

        force = sim->system->forces[i];
        k = sim->forcePrograms[i] = SMProgramLibraryMakeProgram(sim->library,
                                                                force->type);

        forceBuf = COBufferNew(sim->computer, 1, sizeof(PAPhysicsForce), false);
        COBufferSet(forceBuf, force);

        COProgramSetArgument(k, 0,
                             COArgumentNewWithBuffer(sim->clParticles, 0));
        COProgramSetArgument(k, 1,
                             COArgumentNewWithBuffer(sim->clParticles, 1));
        COProgramSetArgument(k, 2,
                             COArgumentNewWithBuffer(sim->clNewtonian, 0));
        COProgramSetArgument(k, 3,
                             COArgumentNewWithBuffer(sim->clNewtonian, 1));
        COProgramSetArgument(k, 4, COArgumentNewWithBuffer(forceBuf, 0));
        COProgramSetArgument(k, 5, COArgumentNewWithInt(sim->elementCount));
    }

    // Load the integrator and set its arguments

    sim->integrationProgram =
        SMProgramLibraryMakeProgram(sim->library, PAPhysicsIntegrationType);

    COProgramSetArgument(sim->integrationProgram, 0,
                         COArgumentNewWithBuffer(sim->clParticles, 0));
    COProgramSetArgument(sim->integrationProgram, 1,
                         COArgumentNewWithBuffer(sim->clParticles, 1));
    COProgramSetArgument(sim->integrationProgram, 2,
                         COArgumentNewWithBuffer(sim->clNewtonian, 0));
    COProgramSetArgument(sim->integrationProgram, 3,
                         COArgumentNewWithBuffer(sim->clNewtonian, 1));
    COProgramSetArgument(sim->integrationProgram, 4,
                         COArgumentNewWithInt(sim->elementCount));

    SMSimulatorRandomize(sim);

    return sim;
}

// copy data from normal memory -> video memory
void SMSimulatorPushData(SMSimulator * sim)
{
    COBufferSet(sim->clParticles, sim->particles);
    COBufferSet(sim->clNewtonian, sim->newtonian);
}

// copy particle locations from video -> normal memory
void SMSimulatorPullData(SMSimulator * sim)
{
    COBufferGet(sim->clParticles, (void**)&sim->particles);
    COBufferGet(sim->clNewtonian, (void**)&sim->newtonian);
}

// randomly choose particle positions, using all available particles
void SMSimulatorRandomize(SMSimulator * sim)
{
    srand((int)time(NULL));

    for(unsigned int i = 0; i < sim->elementCount; i++)
    {
        sim->particles[i].lifetime = 1.0;
        sim->newtonian[i].ox = sim->particles[i].x =
            (PAFloat)rand()/(PAFloat)RAND_MAX;
        sim->newtonian[i].oy = sim->particles[i].y =
            (PAFloat)rand()/(PAFloat)RAND_MAX;
        sim->newtonian[i].oz = sim->particles[i].z =
            (PAFloat)rand()/(PAFloat)RAND_MAX;

        sim->newtonian[i].mass = 50000000.0 * (PAFloat)rand()/(PAFloat)RAND_MAX;
    }
}

void setupEmitterParticle(SMSimulator * sim, int p, PAEmitter * emitter)
{
    sim->particles[p].lifetime = emitter->lifetime;
    sim->particles[p].x = emitter->particle.x;
    sim->particles[p].y = emitter->particle.y;
    sim->particles[p].z = emitter->particle.z;

    sim->newtonian[p].ox = sim->particles[p].x +
        ((PAFloat)rand()/(PAFloat)RAND_MAX - 0.5) * 0.01;
    sim->newtonian[p].oy = sim->particles[p].y +
        ((PAFloat)rand()/(PAFloat)RAND_MAX - 0.5) * 0.01;
    sim->newtonian[p].oz = sim->particles[p].z +
        ((PAFloat)rand()/(PAFloat)RAND_MAX - 0.5) * 0.01;

    sim->newtonian[p].mass = 50000000.0 *
        (PAFloat)rand()/(PAFloat)RAND_MAX;
}

void SMSimulatorSimulate(SMSimulator * sim)
{
    if(sim->system->emitterCount)
        SMSimulatorPullData(sim);

    for(unsigned int i = 0; i < sim->system->emitterCount; i++)
    {
        unsigned int oldCount = sim->elementCount;
        PAEmitter * emitter = sim->system->emitters[i];

        unsigned int neededParticles = emitter->birthRate;

        for(unsigned int p = 0; p < sim->elementCount; p++)
        {
            if(sim->particles[p].lifetime == 0)
            {
                setupEmitterParticle(sim, p, emitter);
                neededParticles--;
            }
        }

        sim->elementCount += neededParticles;

        if(oldCount != sim->elementCount)
        {
            SMProgramLibrarySetGlobalCount(sim->library, sim->elementCount);

            sim->particles = (PAPhysicsParticle *)
                realloc(sim->particles,
                        sim->elementCount * sizeof(PAPhysicsParticle));

            sim->newtonian = (PAPhysicsNewtonian *)
                realloc(sim->newtonian,
                        sim->elementCount * sizeof(PAPhysicsNewtonian));
        }

        for(unsigned int p = oldCount; p < sim->elementCount; p++)
            setupEmitterParticle(sim, p, emitter);

        if(oldCount != sim->elementCount)
        {
            COBufferResize(sim->clParticles, sim->elementCount);
            COBufferResize(sim->clNewtonian, sim->elementCount);

            for(unsigned int j = 0; j < sim->system->forceCount; j++)
            {
                COProgramSetArgument(sim->forcePrograms[j], 5,
                                     COArgumentNewWithInt(sim->elementCount));
            }
            COProgramSetArgument(sim->integrationProgram, 4,
                                 COArgumentNewWithInt(sim->elementCount));
        }
    }

    if(sim->system->emitterCount)
        SMSimulatorPushData(sim);

    for(unsigned int i = 0; i < sim->system->forceCount; i++)
    {
        COProgramExecute(sim->forcePrograms[i]);
        COContextWait(sim->computer);
    }

    COProgramExecute(sim->integrationProgram);
    COContextWait(sim->computer);

    COBufferSwap(sim->clParticles);
    COBufferSwap(sim->clNewtonian);

    SMSimulatorPullData(sim);
}
