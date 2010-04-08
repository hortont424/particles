import os
import Build

SetOption('num_jobs', 4)

VariantDir('Libraries/build', 'Libraries')
VariantDir('Simulator/build', 'Simulator', duplicate = 0)
VariantDir('Interpolator/build', 'Interpolator', duplicate = 0)
VariantDir('Previewer/build', 'Previewer', duplicate = 0)
VariantDir('External/json-c/build', 'External/json-c', duplicate = 0)
VariantDir('Papers/build', 'Papers', duplicate = 0)

includes = ["-ILibraries", "-IExternal/json-c"]
flags = includes + ["--std=c99", "-Wall", "-Werror", "-fobjc-gc"]

releaseFlags = flags + ["-O4"]
debugFlags = flags + ["-g"]

buildMode = ARGUMENTS.get("mode", "release")

if buildMode == "debug":
    useFlags = debugFlags
elif buildMode == "release":
    useFlags = releaseFlags
else:
    print "Error: unknown build mode: %s" % buildMode
    exit()

print "=== Building in %s mode ===" % buildMode

outerEnv = Environment(
    ENV = { "PATH": os.environ['PATH'] },
    CC = "clang",
    CCFLAGS = useFlags,
    LIBPATH = ["#Libraries/build/libcurve",
               "#Libraries/build/libparticles",
               "#Libraries/build/libcomputer",
               "#External/json-c/build"])

Build.InstallTools(outerEnv)
Export("outerEnv")

# Papers
papers = SConscript('Papers/build/SConscript')

# External libraries
libjsonc = SConscript('External/json-c/build/SConscript')

extern = ['libjsonc']

# Libraries
libparticles = SConscript('Libraries/build/libparticles/SConscript', extern)
libcurve = SConscript('Libraries/build/libcurve/SConscript', extern)
libcomputer = SConscript('Libraries/build/libcomputer/SConscript', extern)

libraries = extern + ['libcomputer', 'libparticles', 'libcurve']

# Tools
simulator = SConscript('Simulator/build/SConscript', libraries)
interpolator = SConscript('Interpolator/build/SConscript', libraries)
previewer = SConscript('Previewer/build/SConscript', libraries)