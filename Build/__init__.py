import PrettyPrint
import MacApplication
import WriteVal
import CompileXIB

from SCons.Environment import *

def installTools(env):
    WriteVal.TOOL_WRITE_VAL(env)
    CompileXIB.TOOL_COMPILE_XIB(env)
    MacApplication.TOOL_MAC_APPLICATION(env)
    PrettyPrint.TOOL_PRETTY_PRINT(env)
    return env