import os, os.path

Import('env_lpc1549')
Import('env_stm32f303')

all_firmwares = []

# Environment for building projects for labs, without affecting the base environment
env_lpc1549 = env_lpc1549.Clone()
env_lpc1549.Append(CCFLAGS=['-Wall', '-Werror'])

env_stm32f303 = env_stm32f303.Clone()
env_stm32f303.Append(CCFLAGS=['-Wall', '-Werror'])

# Separately build main.cpp and supporting code, so that the supporting code can
# be shared with solution compilation
main_all_lpc1549 = Glob('src/*.cpp')
support_srcs_lpc1549 = [elt for elt in main_all_lpc1549
    if os.path.basename(elt.abspath) not in ['main.cpp']]
supportlib_lpc1549 = env_lpc1549.StaticLibrary('mainlibs_lpc1549', support_srcs_lpc1549)
env_lpc1549.Append(LIBS=supportlib_lpc1549)

main_all_stm32f303 = Glob('src_stm32f303/*.cpp')
support_srcs_stm32f303 = [elt for elt in main_all_stm32f303
    if os.path.basename(elt.abspath) not in ['main.cpp']]
supportlib_stm32f303 = env_stm32f303.StaticLibrary('mainlibs_stm32f303', support_srcs_stm32f303)
env_stm32f303.Append(LIBS=supportlib_stm32f303)

all_firmwares.append(env_lpc1549.CalSolFW('brain',
  srcs='src/main.cpp',
  includes=['src']
))

all_firmwares.append(env_stm32f303.CalSolFW('stm32f303',
  srcs='src_stm32f303/main.cpp',
  includes=['src_stm32f303']
))

# Build all solutions
solutions = Glob('solutions/*.cpp')
for solution in solutions:
  name = os.path.splitext(solution.name)[0]
  all_firmwares.append(env_lpc1549.CalSolFW(os.path.join('solutions', name),
    srcs=[solution],
    includes=['src']
  ))

solutions = Glob('solutions_stm32f303/*.cpp')
for solution in solutions:
  name = os.path.splitext(solution.name)[0]
  all_firmwares.append(env_stm32f303.CalSolFW(os.path.join('solutions_stm32f303', name),
    srcs=[solution],
    includes=['src_stm32f303']
  ))

# Build the master node with SLCAN debugging
all_firmwares.append(env_lpc1549.CalSolFW('util/lab2slcan',
  srcs=Glob('util/*.cpp'),
  includes=['util','src']
))

Return('all_firmwares')
