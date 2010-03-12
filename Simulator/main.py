from libsimulator import *
from ctypes import *
from random import random
from itertools import izip

PARTICLE_SIZE = sizeof(SMPhysicsParticle)
NEWTONIAN_SIZE = sizeof(SMPhysicsNewtonian)
ELEMENT_COUNT = 2048
FRAME_SIZE = (ELEMENT_COUNT * PARTICLE_SIZE)
FRAME_COUNT = 200
TOTAL_SIZE = (FRAME_SIZE * FRAME_COUNT)
TOTAL_ELEMENTS = (ELEMENT_COUNT * FRAME_COUNT)

sim = SMContextNew()
lib = SMProgramLibraryNew(sim)
SMProgramLibrarySetGlobalCount(lib, ELEMENT_COUNT)

particles = SMBufferNew(sim, ELEMENT_COUNT, PARTICLE_SIZE, True);
newtonian = SMBufferNew(sim, ELEMENT_COUNT, NEWTONIAN_SIZE, True);

output = SMBufferNewWithFile(sim, TOTAL_ELEMENTS, PARTICLE_SIZE, "test.out");
partialResults = SMBufferGetNativeBuffer(output);
print "Created output file ({0} KB)\n".format(TOTAL_SIZE / 1024)

initialParticles = (SMPhysicsParticle * ELEMENT_COUNT)()
initialNewtonian = (SMPhysicsNewtonian * ELEMENT_COUNT)()

for (p, n) in izip(initialParticles, initialNewtonian):
    n.enabled = 1.0
    n.mass = 100000000.0 * random()
    n.ox = p.x = random()
    n.ox = p.y = random()
    n.oy = p.z = random()

SMBufferSet(particles, initialParticles)
SMBufferSet(newtonian, initialNewtonian)
initialParticles = initialNewtonian = 0

#for i in range(0, SMPhysicsLastType):
#    prog = SMProgramLibraryGetProgram(lib, i)
#    SMProgramSetArgument(prog, 0, SMArgumentNewWithBuffer(particles, 0));
#    SMProgramSetArgument(prog, 1, SMArgumentNewWithBuffer(particles, 1));
#    SMProgramSetArgument(prog, 2, SMArgumentNewWithBuffer(newtonian, 0));
#    SMProgramSetArgument(prog, 3, SMArgumentNewWithBuffer(newtonian, 1));
#    SMProgramSetArgument(prog, 4, SMArgumentNewWithInt(ELEMENT_COUNT));