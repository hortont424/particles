import PrettyPrint
import MacApplication
import WriteVal
import NIB
import LaTeX

from SCons.Environment import *

def InstallTools(env):
    WriteVal.TOOL_WRITE_VAL(env)
    NIB.TOOL_NIB(env)
    MacApplication.TOOL_MAC_APPLICATION(env)
    PrettyPrint.TOOL_PRETTY_PRINT(env)
    LaTeX.TOOL_LATEX(env)
    return env