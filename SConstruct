import os
from Build.prettyprint import TOOL_PRETTY_OUTPUT

VariantDir('Simulator/build', 'Simulator', duplicate=0)
VariantDir('Interpolator/build', 'Interpolator', duplicate=0)
VariantDir('Previewer/build', 'Previewer', duplicate=0)
VariantDir('External/json-c/build', 'External/json-c', duplicate=0)

outerEnv = Environment(ENV = {'PATH' : os.environ['PATH']})
TOOL_PRETTY_OUTPUT(outerEnv)
Export("outerEnv")

libjsonc = SConscript('External/json-c/build/SConscript')

libcurve = SConscript('Interpolator/build/libcurve/SConscript')

libsimulator = SConscript('Simulator/build/libsimulator/SConscript')
simulator = SConscript('Simulator/build/SConscript', 'libsimulator')

interpolator = SConscript('Interpolator/build/SConscript')

previewer = SConscript('Previewer/build/SConscript')