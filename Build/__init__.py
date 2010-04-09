import PrettyPrint
import MacApplication
import WriteVal
import NIB

from SCons.Environment import *

def InstallTools(env):
    WriteVal.TOOL_WRITE_VAL(env)
    NIB.TOOL_NIB(env)
    MacApplication.TOOL_MAC_APPLICATION(env)
    PrettyPrint.TOOL_PRETTY_PRINT(env)
    return env