import PrettyPrint
import MacApplication
import WriteVal
import CompileXIB
import LaTeX

from SCons.Environment import *

def InstallTools(env):
    WriteVal.TOOL_WRITE_VAL(env)
    CompileXIB.TOOL_COMPILE_XIB(env)
    MacApplication.TOOL_MAC_APPLICATION(env)
    PrettyPrint.TOOL_PRETTY_PRINT(env)
    LaTeX.TOOL_LATEX(env)
    return env