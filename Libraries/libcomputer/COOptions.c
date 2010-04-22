/* particles - libcomputer - COOptions.c
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

#include "libcomputer.h"

int computerUsesCPU = false;

static struct poptOption optionsTable[] = {
    { "cpu", 'c', 0, &computerUsesCPU, 0, "use CPU for computation" },
    POPT_AUTOHELP
    POPT_TABLEEND
};

/**
 * Parse the given command line options, setting flags relevant to our
 * interests.
 *
 * @todo This should be made much more robust; it should remove arguments from
 * the argc/argv; it should provide -h/--help, etc.
 *
 * @param argc The number of arguments in the argument array.
 * @param argv The command line argument array.
 */
void COOptionsParse(int argc, const char ** argv)
{
    poptContext optionContext;

    optionContext = poptGetContext(NULL, argc, argv, optionsTable, 0);

    while(poptGetNextOpt(optionContext) >= 0);

    poptFreeContext(optionContext);
}
