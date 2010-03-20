#!/usr/bin/env python

import os

from SCons.Builder import *

def TOOL_COMPILE_XIB(env):
    if 'COMPILE_XIB' in env['TOOLS']:
        return
    else:
        env.Append(TOOLS = 'COMPILE_XIB')

    def compile_xib(target, source, env):
        args = "--errors --output-format human-readable-text"

        for plugin in env["IBPLUGINSPATH"]:
            args += " --plugin-dir %s" % plugin

        os.system("ibtool %s --compile %s %s" % (args, target[0], source[0]))

    env['BUILDERS']['CompileXIB'] = Builder(
        action = compile_xib,
        suffix = ".nib",
        src_suffix = ".xib")
