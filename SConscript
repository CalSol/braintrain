import os.path

Import('env_lpc1549')

env = env_lpc1549.Clone()

env.Append(CCFLAGS=['-Wall', '-Werror'])

# Separately build main.cpp and supporting code, so that the supporting code can
# be shared with solution compilation
main_all = Glob('src/*.cpp')
main_srcs = [elt for elt in main_all if elt.name == 'main.cpp']
mainlibs_srcs = [elt for elt in main_all if elt not in main_srcs]

mainlibs = env.StaticLibrary('mainlibs', mainlibs_srcs)
env.Append(LIBS=mainlibs)

env.Default(env.CalSolFW('brain',
  srcs=main_srcs,
  includes=['src']
))

# Build all solutions
solutions = Glob('solutions/*.cpp')
for solution in solutions:
  env.Default(env.CalSolFW(
    os.path.join('solutions', os.path.splitext(solution.name)[0]),
    srcs=[solution],
    includes=['src']
  ))
  
env.Alias('prog', 
  env.Command('openocd', 'brain.elf', 
    'openocd -f interface/cmsis-dap.cfg -f lpc1549_openocd.cfg -c init -c "reset halt" -c "flash erase_sector 0 0 last" -c "flash write_image $SOURCE" -c "reset run" -c "exit"'
))
