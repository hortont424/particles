import os
import Build

SetOption('num_jobs', 4)

VariantDir('Libraries/build', 'Libraries')
VariantDir('Simulator/build', 'Simulator', duplicate = 0)
VariantDir('Interpolator/build', 'Interpolator', duplicate = 0)
VariantDir('Previewer/build', 'Previewer', duplicate = 0)
VariantDir('External/json-c/build', 'External/json-c', duplicate = 0)

releaseFlags = ["--std=c99", "-O4", "-Wall", "-Werror", "-fobjc-gc", "-ILibraries"]
debugFlags = ["--std=c99", "-g", "-Wall", "-Werror", "-fobjc-gc", "-ILibraries"]

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
    LIBPATH = Glob("Libraries/build/*"))

Build.InstallTools(outerEnv)
Export("outerEnv")

# External libraries
libjsonc = SConscript('External/json-c/build/SConscript')

# Libraries
libparticles = SConscript('Libraries/build/libparticles/SConscript')
libcurve = SConscript('Libraries/build/libcurve/SConscript')
libsimulator = SConscript('Libraries/build/libsimulator/SConscript')

# Tools
simulator = SConscript('Simulator/build/SConscript', ['libsimulator', 'libparticles'])
interpolator = SConscript('Interpolator/build/SConscript')
previewer = SConscript('Previewer/build/SConscript')