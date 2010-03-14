#ifndef _CU_CURVE_H_
#define _CU_CURVE_H_

typedef enum
{
    CU_CONTROL_POINT_SMOOTH,
    CU_CONTROL_POINT_SMOOTH_SYMMETRIC,
    CU_CONTROL_POINT_CORNER,

    CU_CONTROL_POINT_SENTRY
} CUControlPointType;

typedef struct _CUPoint
{
    float x, y;
} CUPoint;

typedef struct _CUControlPoint
{
    CUControlPointType type;

    CUPoint point;
    CUPoint controlPoints[2];
} CUControlPoint;

typedef struct _CUCurve
{
    CUControlPoint * controlPoints;
    CUControlPoint * currentControlPoint;

    uint32_t pointCount;
} CUCurve;

CUPoint CUPointMake(float x, float y);

CUCurve * CUCurveNew(uint32_t pointCount);
CUCurve * CUCurveNewFromFile(char * filename);
void CUCurveFree(CUCurve * curve);

void CUCurveSave(CUCurve * curve, char * filename);
void CUCurveClear(CUCurve * curve);
void CUCurvePush(CUCurve * curve, CUPoint p, CUPoint cp1, CUPoint cp2);

#endif
