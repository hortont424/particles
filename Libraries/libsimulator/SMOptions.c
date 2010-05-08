/* particles - libsimulator - SMOptions.c
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <popt.h>

#include <liblog/liblog.h>

#include "libsimulator.h"

int simulatorOutputMode = SM_RENDERER_OUTPUT;

static int simulatorPreviewMode = false;
static int simulatorBenchmarkMode = false;
static int simulatorRenderMode = false;

static struct poptOption optionsTable[] = {
    { "preview", 'p', 0, &simulatorPreviewMode, 0, "preview output" },
    { "benchmark", 'b', 0, &simulatorBenchmarkMode, 0, "benchmark simulation" },
    { "render", 'r', 0, &simulatorRenderMode, 0, "render output" },
    POPT_AUTOHELP
    POPT_TABLEEND
};

void SMOptionsParse(int argc, const char ** argv)
{
    poptContext optionContext;

    optionContext = poptGetContext(NULL, argc, argv, optionsTable, 0);

    while(poptGetNextOpt(optionContext) >= 0);

    if(simulatorPreviewMode)
        simulatorOutputMode = SM_PREVIEWER_OUTPUT;
    if(simulatorRenderMode)
        simulatorOutputMode = SM_RENDERER_OUTPUT;
    else if(simulatorBenchmarkMode)
        simulatorOutputMode = SM_NO_OUTPUT;

    poptFreeContext(optionContext);
}
