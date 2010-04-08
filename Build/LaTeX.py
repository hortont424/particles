#!/usr/bin/env python

import os

from SCons.Action import *
from SCons.Builder import *

def TOOL_LATEX(env):
    if 'LaTeX' in env['TOOLS']:
        print "lalalalalala"
        return
    else:
        env.Append(TOOLS = 'LaTeX')

    def LaTeX(target, source, env):
        flags = "-interaction=nonstopmode"

        if os.system("pdflatex %s %s > /tmp/latexoutput" % (flags, source[0])):
            os.system("cat /tmp/latexoutput")

    env['BUILDERS']['LaTeX'] = Builder(
        action = Action(LaTeX, "$LATEXCOMSTR"),
        suffix = ".pdf",
        src_suffix = ".latex")
