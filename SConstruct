# Top-level build wrapper so build outputs go in a separate directory.
import multiprocessing
import os
import os.path

SetOption('num_jobs', multiprocessing.cpu_count() + 1)

# On Windows platforms, force gcc-style (instead of MSVC-style) arguments.
env = Environment(ENV=os.environ, tools=['g++', 'gcc', 'gas', 'ar', 'gnulink'])

# Build wrapper for firmware that eliminates problematic files and
# automatically builds the .elf, .bin, and dumps debugging data
def calsol_fw(env, target, srcs, includes):
  ignores = ['cr_startup_lpc11.cpp']
  srcs = [f for f in srcs if f.name not in ignores]
  prog = env.Program(target, srcs,
      CPPPATH=env['CPPPATH'] + includes,
  )
  bin = env.Binary(prog)
  size = env.SymbolsSize(prog)
  dump = env.Objdump(prog)
  all = env.Alias(target, [prog, bin, size, dump])
  return all
env.AddMethod(calsol_fw, "CalSolFW")

def simplify_output(env, mappings):
  pad_len = max([len(val) for val in mappings.values()]) + 2
  for key, val in mappings.items():
    env[key] = val + (' ' * (pad_len - len(val))) + '$TARGET'

if ARGUMENTS.get('VERBOSE') != '1':
  simplify_output(env, {
    'ASPPCOMSTR': 'AS',
    'ASCOMSTR': 'AS',
    'ARCOMSTR': 'AR',
    'CCCOMSTR': 'CC',
    'CXXCOMSTR': 'CXX',
    'LINKCOMSTR': 'LD',
    'RANLIBCOMSTR': 'IND',
    'BINCOMSTR': 'BIN',
    'OBJDUMPCOMSTR': 'DUMP',
    'SYMBOLSCOMSTR': 'DUMP',
    'SYMBOLSIZESCOMSTR': 'DUMP',
  })

###
### Imports
###
SConscript('mbed-scons/SConscript-mbed', exports=['env'], duplicate=0)
SConscript('mbed-scons/SConscript-env-gcc-arm', exports=['env'], duplicate=0)

###
### Global configurations
###
env.Append(CCFLAGS=[
  '-O1',
  '-g',
])
env.Append(LINKFLAGS=[
  '--specs=nosys.specs',
  '-u', '_printf_float',
])

###
### Platform-specific build targets for mbed libraries
###
# TODO: perhaps move environments into SConscript, but nested variant_dirs
# don't seem to work
env_orig = env

env = env_orig.Clone()
SConscript('SConscript-env-lpc1549', variant_dir='build/lpc1549',
    exports=['env'], duplicate=0)
env_lpc1549 = env

###
### Actual build targets here
###
SConscript('SConscript', variant_dir='build', 
    exports=['env_lpc1549'], duplicate=0)
