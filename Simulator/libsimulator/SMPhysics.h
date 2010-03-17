/* particles - libsimulator - SMPhysics.h
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

#ifndef _SM_PHYSICS_H_
#define _SM_PHYSICS_H_

#ifndef cl_float
typedef float SMFloat;
typedef int SMInt;
#else
typedef cl_float SMFloat;
typedef cl_int SMInt;
#endif

// Types of different supported forces
typedef enum _SMPhysicsType
{
    SMPhysicsNormalType,
    SMPhysicsWindType,
    SMPhysicsVortexType,
    SMPhysicsHarmonicType,
    SMPhysicsGravityType,
    SMPhysicsDragType,
    SMPhysicsTurbulenceType,
    SMPhysicsBoidsType,

    SMPhysicsIntegrationType,

    SMPhysicsLastType
} SMPhysicsType;

// Falloff attributes
typedef struct _SMPhysicsFalloffData
{
    SMFloat strength, max, min;
} SMPhysicsFalloffData;

// Attributes for various different forces
typedef struct _SMPhysicsNormalData
{
    SMFloat strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsNormalData;

typedef struct _SMPhysicsWindData
{
    SMFloat strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsWindData;

typedef struct _SMPhysicsVortexData
{
    SMFloat strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsVortexData;

typedef struct _SMPhysicsHarmonicData
{
    SMFloat strength, damping, restLength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsHarmonicData;

typedef struct _SMPhysicsGravityData
{
    SMFloat strength, noise;
} SMPhysicsGravityData;

typedef struct _SMPhysicsDragData
{
    SMFloat linearStrength, quadraticStrength, noise;
} SMPhysicsDragData;

typedef struct _SMPhysicsTurbulenceData
{
    SMFloat strength, size, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsTurbulenceData;

typedef struct _SMPhysicsBoidsData
{
    SMFloat strength, noise;
} SMPhysicsBoidsData;

// There should be one SMPhysicsForce for each particle that exerts a force
// on others, mapped to said particle (so that force particles can be interacted
// with, and move just like other particles)
typedef struct _SMPhysicsForce
{
    SMFloat particleIndex;   // These need to not be SMFloats, but I'm afraid
    SMFloat forceType;       // of what will happen if they're not the same size
                             // as everything else when being passed in...
    union
    {
        SMPhysicsNormalData normal;
        SMPhysicsWindData wind;
        SMPhysicsVortexData vortex;
        SMPhysicsHarmonicData harmonic;
        SMPhysicsGravityData gravity;
        SMPhysicsDragData drag;
        SMPhysicsTurbulenceData turbulence;
        SMPhysicsBoidsData boids;
    } forceData;
} SMPhysicsForce;

// There should be one SMPhysicsNewtonian for each particle in the system; this
// holds universal properties like mass, velocity, and acceleration.
typedef struct _SMPhysicsNewtonian
{
    SMFloat fixed, mass;
    SMFloat ax, ay, az;
    SMFloat ox, oy, oz;
} SMPhysicsNewtonian;

// Basic particle data; all you need to do the most simplistic rendering
typedef struct _SMPhysicsParticle
{
    SMFloat enabled;
    SMFloat x, y, z;
} SMPhysicsParticle;

#endif
