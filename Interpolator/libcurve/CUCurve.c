#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "Curve.h"

CUPoint CUPointMake(float x, float y)
{
    CUPoint p;
    p.x = x;
    p.y = y;
    return p;
}

CUCurve * CUCurveNew(unsigned int pointCount)
{
    CUCurve * curve;
    curve = (CUCurve *)calloc(1, sizeof(CUCurve));

    curve->pointCount = pointCount;
    curve->controlPoints = (CUControlPoint *)calloc(pointCount,
                                                    sizeof(CUControlPoint));
    curve->currentControlPoint = curve->controlPoints;

    return curve;
}

void CUCurveFree(CUCurve * curve)
{
    free(curve->controlPoints);
    free(curve);
}

void CUCurveClear(CUCurve * curve)
{
    curve->currentControlPoint = curve->controlPoints;
}

void CUCurvePush(CUCurve * curve, CUPoint p, CUPoint cp1, CUPoint cp2)
{
    if(curve->currentControlPoint >= curve->controlPoints + curve->pointCount)
    {
        printf("Trying to push more than %d points.\n", curve->pointCount);
        return;
    }

    curve->currentControlPoint->point = p;
    curve->currentControlPoint->controlPoints[0] = cp1;
    curve->currentControlPoint->controlPoints[1] = cp2;

    curve->currentControlPoint++;
}
