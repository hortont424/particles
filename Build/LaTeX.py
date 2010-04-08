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
        output = os.path.dirname(str(target[0]))

        def callLaTeX():
            os.system("pdflatex --output-directory %s %s %s > /tmp/ltxout" %
                (output, flags, source[0]))

        callLaTeX()
        if(callLaTeX()):
            os.system("cat /tmp/ltxout")

    env['BUILDERS']['LaTeX'] = Builder(
        action = Action(LaTeX, "$LATEXCOMSTR"),
        suffix = ".pdf",
        src_suffix = ".latex")
