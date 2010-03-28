/* particles - libcurve - CUCurve.h
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
