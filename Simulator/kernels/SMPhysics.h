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
    double strength, max, min;
} SMPhysicsFalloffData

// Attributes for various different forces
typedef struct _SMPhysicsNormalData
{
    double strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsNormalData;

typedef struct _SMPhysicsWindData
{
    double strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsWindData;

typedef struct _SMPhysicsVortexData
{
    double strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsVortexData;

typedef struct _SMPhysicsHarmonicData
{
    double strength, damping, restLength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsHarmonicData;

typedef struct _SMPhysicsGravityData
{
    double strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsGravityData;

typedef struct _SMPhysicsDragData
{
    double linearStrength, quadraticStrength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsDragData;

typedef struct _SMPhysicsTurbulenceData
{
    double strength, size, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsTurbulenceData;

typedef struct _SMPhysicsBoidsData
{
    double strength, noise;
    SMPhysicsFalloffData falloff;
} SMPhysicsBoidsData;

// Overarching force struct (with typed-union attribute blob)
typedef struct _SMPhysicsForce
{
    double particleIndex;   // These need to not be doubles, but I'm afraid
    double forceType;       // of what will happen if they're not the same size
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

#endif