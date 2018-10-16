import os, os.path

Import('env_lpc1549')

all_firmwares = []

# Environment for building projects for labs
env = env_lpc1549.Clone()

env.Append(CCFLAGS=['-Wall', '-Werror'])

# Separately build main.cpp and supporting code, so that the supporting code can
# be shared with solution compilation
main_all = Glob('src/*.cpp')
main_srcs = [elt for elt in main_all if elt.name == 'main.cpp']
support_srcs = [elt for elt in main_all if elt not in main_srcs]

supportlib = env.StaticLibrary('mainlibs', support_srcs)
env.Append(LIBS=supportlib)

all_firmwares.append(env.CalSolFW('brain',
  srcs=main_srcs,
  includes=['src']
))

# Build all solutions
solutions = Glob('solutions/*.cpp')
for solution in solutions:
  name = os.path.splitext(solution.name)[0]
  all_firmwares.append(env.CalSolFW(os.path.join('solutions', name),
    srcs=[solution],
    includes=['src']
  ))

# Build the master node with SLCAN debugging
all_firmwares.append(env.CalSolFW('util/lab2slcan',
  srcs=Glob('util/*.cpp'),
  includes=['util','src']
))

Return('all_firmwares')
