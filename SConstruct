import os
import Build

SetOption('num_jobs', 4)

VariantDir('Libraries/build', 'Libraries')
VariantDir('Simulator/build', 'Simulator', duplicate = 0)
VariantDir('Interpolator/build', 'Interpolator', duplicate = 0)
VariantDir('Previewer/build', 'Previewer', duplicate = 0)
VariantDir('External/json-c/build', 'External/json-c', duplicate = 0)

flags = ["--std=c99", "-Wall", "-Werror", "-fobjc-gc", "-ILibraries"]

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
               "#Libraries/build/libcomputer"])

Build.InstallTools(outerEnv)
Export("outerEnv")

# External libraries
libjsonc = SConscript('External/json-c/build/SConscript')

# Libraries
libparticles = SConscript('Libraries/build/libparticles/SConscript')
libcurve = SConscript('Libraries/build/libcurve/SConscript')
libcomputer = SConscript('Libraries/build/libcomputer/SConscript')

libraries = ['libcomputer', 'libparticles', 'libcurve', 'libjsonc']

# Tools
simulator = SConscript('Simulator/build/SConscript', libraries)
interpolator = SConscript('Interpolator/build/SConscript', libraries)
previewer = SConscript('Previewer/build/SConscript', libraries)