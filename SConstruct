import os
import Build

SetOption('num_jobs', 4)

VariantDir('Libraries/build', 'Libraries')
VariantDir('Simulator/build', 'Simulator', duplicate = 0)
VariantDir('Interpolator/build', 'Interpolator', duplicate = 0)
VariantDir('Previewer/build', 'Previewer', duplicate = 0)
VariantDir('External/json-c/build', 'External/json-c', duplicate = 0)
VariantDir('Papers/build', 'Papers', duplicate = 0)

includes = ["-ILibraries", "-IExternal/json-c", "-I/opt/local/include"]
flags = includes + ["-std=c99", "-Wall", "-Werror", "-fobjc-gc"]

releaseFlags = flags + ["-O4"]
debugFlags = flags + ["-g"]

buildMode = ARGUMENTS.get("mode", "debug")

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
               "#Libraries/build/librenderer",
               "#Libraries/build/libpreviewer",
               "#Libraries/build/libsimulator",
               "#External/json-c/build",
               "/opt/local/lib"])

Build.InstallTools(outerEnv)
Export("outerEnv")

# Papers
#papers = SConscript('Papers/build/SConscript')

# External libraries
libjsonc = SConscript('External/json-c/build/SConscript')
libraries = ['libjsonc']

# Base Libraries
libparticles = SConscript('Libraries/build/libparticles/SConscript', libraries)
libcurve = SConscript('Libraries/build/libcurve/SConscript', libraries)
libcomputer = SConscript('Libraries/build/libcomputer/SConscript', libraries)
libraries += ['libcomputer', 'libparticles', 'libcurve']

# High-level Libraries
librenderer = SConscript('Libraries/build/librenderer/SConscript', libraries)
libpreviewer = SConscript('Libraries/build/libpreviewer/SConscript', libraries)
libsimulator = SConscript('Libraries/build/libsimulator/SConscript', libraries)
libraries += ['librenderer', 'libpreviewer', 'libsimulator']

# Tools
simulator = SConscript('Simulator/build/SConscript', libraries)
interpolator = SConscript('Interpolator/build/SConscript', libraries)

outerEnv.Install('/usr/local/lib', [libjsonc, libparticles, libcurve, libcomputer, librenderer, libpreviewer, libsimulator])
outerEnv.Alias('install', '/usr/local/lib')
