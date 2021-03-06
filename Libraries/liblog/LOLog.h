/* particles - liblog - LOLog.h
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

#ifndef _LO_LOG_H_
#define _LO_LOG_H_

#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>

#define LO_BOLD     "\033[1m"
#define LO_RED      "\033[91m"
#define LO_RESET    "\033[0m"

#define LOLog(f, ...) \
{ \
    printf(LO_BOLD "(%s, line %d): " LO_RESET f "\n", \
           basename(__FILE__), __LINE__, ##__VA_ARGS__); \
}

#define LOError(f, ...) \
{ \
    fprintf(stderr, LO_BOLD LO_RED "Error (%s, line %d): " LO_RESET f "\n", \
            basename(__FILE__), __LINE__, ##__VA_ARGS__); \
    exit(EXIT_FAILURE); \
}

#endif
