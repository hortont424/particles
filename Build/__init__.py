import prettyprint
import macosx

from SCons.Environment import *

def installTools(env):
    macosx.TOOL_BUNDLE(env)
    prettyprint.TOOL_PRETTY_OUTPUT(env)
    return env