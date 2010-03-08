VariantDir('Simulator/build', 'Simulator', duplicate=0)
VariantDir('Interpolator/build', 'Interpolator', duplicate=0)

libsimulator = SConscript('Simulator/build/libsimulator/SConscript')
simulator = SConscript('Simulator/build/SConscript', 'libsimulator')

interpolator = SConscript('Interpolator/build/SConscript')