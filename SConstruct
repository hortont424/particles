import os
import Build

SetOption('num_jobs', 4)

VariantDir('Simulator/build', 'Simulator', duplicate = 0)
VariantDir('Interpolator/build', 'Interpolator', duplicate = 0)
VariantDir('Previewer/build', 'Previewer', duplicate = 0)
VariantDir('External/json-c/build', 'External/json-c', duplicate = 0)

outerEnv = Environment(ENV = {'PATH' : os.environ['PATH']},
                       CC = "clang",
                       CCFLAGS = "--std=c99 -O4 -Wall -Werror -fobjc-gc")

Build.InstallTools(outerEnv)
Export("outerEnv")

libjsonc = SConscript('External/json-c/build/SConscript')

libcurve = SConscript('Interpolator/build/libcurve/SConscript')

libsimulator = SConscript('Simulator/build/libsimulator/SConscript')
simulator = SConscript('Simulator/build/SConscript', 'libsimulator')

interpolator = SConscript('Interpolator/build/SConscript')

previewer = SConscript('Previewer/build/SConscript')