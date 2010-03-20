import os
import Build

SetOption('num_jobs', 4)

VariantDir('Simulator/build', 'Simulator', duplicate = 0)
VariantDir('Interpolator/build', 'Interpolator', duplicate = 0)
VariantDir('Previewer/build', 'Previewer', duplicate = 0)
VariantDir('External/json-c/build', 'External/json-c', duplicate = 0)

releaseFlags = ["--std=c99", "-O4", "-Wall", "-Werror", "-fobjc-gc"]
debugFlags = ["--std=c99", "-g", "-Wall", "-Werror", "-fobjc-gc"]

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
    ENV = {'PATH' : os.environ['PATH']},
    CC = "clang",
    CCFLAGS = useFlags)

Build.InstallTools(outerEnv)
Export("outerEnv")

libjsonc = SConscript('External/json-c/build/SConscript')

libcurve = SConscript('Interpolator/build/libcurve/SConscript')

libsimulator = SConscript('Simulator/build/libsimulator/SConscript')
simulator = SConscript('Simulator/build/SConscript', 'libsimulator')

interpolator = SConscript('Interpolator/build/SConscript')

previewer = SConscript('Previewer/build/SConscript')