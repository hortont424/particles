/* particles - libparticles - PAPhysics.h
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

#ifndef _PA_PHYSICS_H_
#define _PA_PHYSICS_H_

#include "PATypes.h"

// Types of different supported forces
typedef enum _PAPhysicsType
{
    PAPhysicsNormalType,
    PAPhysicsWindType,
    PAPhysicsVortexType,
    PAPhysicsHarmonicType,
    PAPhysicsGravityType,
    PAPhysicsDragType,
    PAPhysicsTurbulenceType,
    PAPhysicsBoidsType,

    PAPhysicsIntegrationType,

    PAPhysicsLastType
} PAPhysicsType;

// Falloff attributes
typedef struct _PAPhysicsFalloffData
{
    PAFloat strength, max, min;
} PAPhysicsFalloffData;

// Attributes for various different forces
typedef struct _PAPhysicsNormalData
{
    PAFloat strength, noise;
    PAPhysicsFalloffData falloff;
} PAPhysicsNormalData;

typedef struct _PAPhysicsWindData
{
    PAFloat strength, noise;
    PAPhysicsFalloffData falloff;
} PAPhysicsWindData;

typedef struct _PAPhysicsVortexData
{
    PAFloat strength, noise;
    PAPhysicsFalloffData falloff;
} PAPhysicsVortexData;

typedef struct _PAPhysicsHarmonicData
{
    PAFloat strength, damping, restLength, noise;
    PAPhysicsFalloffData falloff;
} PAPhysicsHarmonicData;

typedef struct _PAPhysicsGravityData
{
    PAFloat strength, noise;
} PAPhysicsGravityData;

typedef struct _PAPhysicsDragData
{
    PAFloat linearStrength, quadraticStrength, noise;
} PAPhysicsDragData;

typedef struct _PAPhysicsTurbulenceData
{
    PAFloat strength, size, noise;
    PAPhysicsFalloffData falloff;
} PAPhysicsTurbulenceData;

typedef struct _PAPhysicsBoidsData
{
    PAFloat strength, noise;
} PAPhysicsBoidsData;

// There should be one PAPhysicsForce for each particle that exerts a force
// on others, mapped to said particle (so that force particles can be interacted
// with, and move just like other particles)
typedef struct _PAPhysicsForce
{
    PAFloat particleIndex;   // These need to not be PAFloats, but I'm afraid
    PAFloat forceType;       // of what will happen if they're not the same size
                             // as everything else when being passed in...
    union
    {
        PAPhysicsNormalData normal;
        PAPhysicsWindData wind;
        PAPhysicsVortexData vortex;
        PAPhysicsHarmonicData harmonic;
        PAPhysicsGravityData gravity;
        PAPhysicsDragData drag;
        PAPhysicsTurbulenceData turbulence;
        PAPhysicsBoidsData boids;
    } forceData;
} PAPhysicsForce;

// There should be one PAPhysicsNewtonian for each particle in the system; this
// holds universal properties like mass, velocity, and acceleration.
typedef struct _PAPhysicsNewtonian
{
    PAFloat fixed, mass;
    PAFloat ax, ay, az;
    PAFloat ox, oy, oz;
} PAPhysicsNewtonian;

#endif
