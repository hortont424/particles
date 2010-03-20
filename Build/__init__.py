import prettyprint
import macosx
import writeval
import compilexib

from SCons.Environment import *

def installTools(env):
    writeval.TOOL_WRITE_VAL(env)
    compilexib.TOOL_COMPILE_XIB(env)
    macosx.TOOL_BUNDLE(env)
    prettyprint.TOOL_PRETTY_PRINT(env)
    return env