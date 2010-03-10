#ifndef _SM_PHYSICS_H_
#define _SM_PHYSICS_H_

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
    float strength, max, min;
} SMPhysicsFalloffData;

// Attributes for various different forces
typedef struct _SMPhysicsNormalData
{
    float strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsNormalData;

typedef struct _SMPhysicsWindData
{
    float strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsWindData;

typedef struct _SMPhysicsVortexData
{
    float strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsVortexData;

typedef struct _SMPhysicsHarmonicData
{
    float strength, damping, restLength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsHarmonicData;

typedef struct _SMPhysicsGravityData
{
    float strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsGravityData;

typedef struct _SMPhysicsDragData
{
    float linearStrength, quadraticStrength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsDragData;

typedef struct _SMPhysicsTurbulenceData
{
    float strength, size, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsTurbulenceData;

typedef struct _SMPhysicsBoidsData
{
    float strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsBoidsData;

// There should be one SMPhysicsForce for each particle that exerts a force
// on others, mapped to said particle (so that force particles can be interacted
// with, and move just like other particles)
typedef struct _SMPhysicsForce
{
    float particleIndex;   // These need to not be floats, but I'm afraid
    float forceType;       // of what will happen if they're not the same size
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
    float fixed, mass;
    float velocityX, velocityY, velocityZ;
    float accelerationX, accelerationY, accelerationZ;
} SMPhysicsNewtonian;

// Basic particle data; all you need to do the most simplistic rendering
typedef struct _SMPhysicsParticle
{
    float enabled;
    float x, y, z;
} SMPhysicsParticle;

#endif
