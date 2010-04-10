/* particles - libcurve - CUCurve.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <fcntl.h>

#include "libcurve.h"

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
        LOLog("'%s' is not a curve file", filename);
        return NULL;
    }

    fileHandle = open(filename, O_RDONLY);

    if(fileHandle == -1)
    {
        LOLog("Could not open '%s'", filename);
        return NULL;
    }

    /// \todo ugh. I hate input-side code. this is absolutely horrible.

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
        LOLog("'%s' is not a curve file", filename);
        return;
    }

    fileHandle = open(filename, O_RDWR | O_CREAT | O_TRUNC);

    if(fileHandle == -1)
    {
        LOLog("Could not open '%s'", filename);
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
        LOLog("Trying to push more than %d points.\n", curve->pointCount);
        return;
    }

    curve->currentControlPoint->point = p;
    curve->currentControlPoint->controlPoints[0] = cp1;
    curve->currentControlPoint->controlPoints[1] = cp2;

    curve->currentControlPoint++;
}
