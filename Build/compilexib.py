#!/usr/bin/env python

from SCons.Builder import *

def TOOL_COMPILE_XIB(env):
    if 'COMPILE_XIB' in env['TOOLS']:
        return
    else:
        env.Append(TOOLS = 'COMPILE_XIB')
####env["IBPLUGINS"]
    env['BUILDERS']['CompileXIB'] = Builder(
        action = "ibtool --errors --warnings --output-format human-readable-text --compile $TARGET $SOURCE",
        suffix = ".nib",
        src_suffix = ".xib")
