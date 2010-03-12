#ifndef _CU_CURVE_H_
#define _CU_CURVE_H_

typedef enum
{
    CU_CONTROL_POINT_SMOOTH,
    CU_CONTROL_POINT_SMOOTH_SYMMETRIC,
    CU_CONTROL_POINT_CORNER,

    CU_CONTROL_POINT_SENTRY
} CUControlPointType;

typedef struct _CUControlPoint
{
    CUControlPointType type;


} CUControlPoint;

typedef struct _CUCurve
{
    CUControlPoint * controlPoints;
} CUCurve;

CUCurve * CUCurveNew(int pointCount);

#endif
