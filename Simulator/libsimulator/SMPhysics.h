#ifndef _SM_PHYSICS_H_
#define _SM_PHYSICS_H_

#ifndef cl_float
typedef float SMFloat;
#else
typedef cl_float SMFloat;
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
    SMPhysicsBoidsType
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
    SMPhysicsFalloffData falloff;
} SMPhysicsGravityData;

typedef struct _SMPhysicsDragData
{
    SMFloat linearStrength, quadraticStrength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsDragData;

typedef struct _SMPhysicsTurbulenceData
{
    SMFloat strength, size, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsTurbulenceData;

typedef struct _SMPhysicsBoidsData
{
    SMFloat strength, noise;
    SMPhysicsFalloffData falloff;
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
    SMFloat vx, vy, vz;
    SMFloat ax, ay, az;
} SMPhysicsNewtonian;

// Basic particle data; all you need to do the most simplistic rendering
typedef struct _SMPhysicsParticle
{
    SMFloat enabled;
    SMFloat x, y, z;
} SMPhysicsParticle;

#endif
