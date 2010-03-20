from SCons.Environment import *

def TOOL_PRETTY_OUTPUT(env):
    greenColor = '\033[92m'
    yellowColor = '\033[93m'
    resetColor = '\033[0m'

    def message(mes, source=True):
        if source:
            color = greenColor
            fileVar = '$SOURCE'
        else:
            color = yellowColor
            fileVar = '$TARGET\n'

        return '%s%s: %s%s' % (color, mes, resetColor, fileVar)

    env.Replace(
        CCCOMSTR = message("Compiling object"),
        SHCCCOMSTR = message("Compiling shared object"),
        ARCOMSTR = message("Linking static library", False),
        SHLINKCOMSTR = message("Linking shared library", False),
        LINKCOMSTR = message("Linking program", False),
        INSTALLSTR = message("Installing", False)
    )
    return env