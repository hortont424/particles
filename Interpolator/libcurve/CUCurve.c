#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <fcntl.h>

#include "Curve.h"

CUPoint CUPointMake(float x, float y)
{
    CUPoint p;
    p.x = x;
    p.y = y;
    return p;
}

CUCurve * CUCurveNew(uint32_t pointCount)
{
    CUCurve * curve;
    curve = (CUCurve *)calloc(1, sizeof(CUCurve));

    curve->pointCount = pointCount;
    curve->controlPoints = (CUControlPoint *)calloc(pointCount,
                                                    sizeof(CUControlPoint));
    curve->currentControlPoint = curve->controlPoints;

    return curve;
}

CUCurve * CUCurveNewFromFile(char * filename)
{
    int fileHandle;
    CUCurve * curve;

    curve = (CUCurve *)calloc(1, sizeof(CUCurve));

    if(!strstr(filename, ".curve"))
    {
        printf("'%s' is not a curve file", filename);
        return NULL;
    }

    fileHandle = open(filename, O_RDONLY);

    if(fileHandle == -1)
    {
        printf("Could not open '%s'", filename);
        return NULL;
    }

    read(fileHandle, &curve->pointCount, sizeof(uint32_t));

    curve->controlPoints = (CUControlPoint *)calloc(curve->pointCount,
                                                    sizeof(CUControlPoint));
    curve->currentControlPoint = curve->controlPoints;

    for(uint32_t i = 0; i < curve->pointCount; i++)
        read(fileHandle, &curve->controlPoints[i], sizeof(CUControlPoint));

    close(fileHandle);

    return curve;
}

void CUCurveFree(CUCurve * curve)
{
    free(curve->controlPoints);
    free(curve);
}

void CUCurveSave(CUCurve * curve, char * filename)
{
    int fileHandle;

    if(!strstr(filename, ".curve"))
    {
        printf("'%s' is not a curve file", filename);
        return;
    }

    fileHandle = open(filename, O_RDWR | O_CREAT | O_TRUNC);

    if(fileHandle == -1)
    {
        printf("Could not open '%s'", filename);
        return;
    }

    write(fileHandle, &curve->pointCount, sizeof(uint32_t));

    for(uint32_t i = 0; i < curve->pointCount; i++)
        write(fileHandle, &curve->controlPoints[i], sizeof(CUControlPoint));

    close(fileHandle);
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
