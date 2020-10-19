# Top-level build wrapper so build outputs go in a separate directory.
import multiprocessing
import os
import os.path
import sys
import subprocess

import warnings, SCons.Errors

SetOption('num_jobs', multiprocessing.cpu_count() + 1)

# On Windows platforms, force gcc-style (instead of MSVC-style) arguments.
env = Environment(ENV=os.environ, tools=['g++', 'gcc', 'gas', 'ar', 'gnulink'])

# On Windows platforms, use a custom spawn
# Copied from https://github.com/SCons/scons/wiki/LongCmdLinesOnWin32
if env['PLATFORM'] == 'win32':
  import win32file
  import win32event
  import win32process
  import win32security

  def my_spawn(sh, escape, cmd, args, spawnenv):
#    print(spawnenv)
#    for var in spawnenv:
#      print(spawnenv[var])
#      spawnenv[var] = spawnenv[var].encode('ascii', 'replace')

    sAttrs = win32security.SECURITY_ATTRIBUTES()
    StartupInfo = win32process.STARTUPINFO()
    newargs = ' '.join(map(escape, args[1:]))
    cmdline = cmd + " " + newargs

    # check for any special operating system commands
    if cmd == 'del':
      for arg in args[1:]:
          win32file.DeleteFile(arg)
      exit_code = 0
    else:
      # otherwise execute the command.
      hProcess, hThread, dwPid, dwTid = win32process.CreateProcess(None, cmdline, None, None, 1, 0, spawnenv, None, StartupInfo)
      win32event.WaitForSingleObject(hProcess, win32event.INFINITE)
      exit_code = win32process.GetExitCodeProcess(hProcess)
      win32file.CloseHandle(hProcess);
      win32file.CloseHandle(hThread);
    return exit_code

  env['SPAWN'] = my_spawn

# Performance improvements
AddOption(
  '--fast',
  action='store_true',
  help='Optimized build that can be inaccurate in some uncommon scenarios.'
)
if GetOption('fast'):
  env.Decider('MD5-timestamp')  # may not catch changes under 1 second apart
  SetOption('implicit_cache', 1)  # may not catch some dependencies, like certain effects of CPPPATH changes

###
### Imports
###
if not os.path.isfile(File('calsol-fw-libs/SConscript-git-utils').abspath):
  raise SCons.Errors.UserError(
      "Missing calsol-fw-libs/SConscript-git-utils, this probably means that the submodule calsol-fw-libs is not initialized. " +
      "Run 'git submodule update --init --recursive' to initialize.")

SConscript('calsol-fw-libs/SConscript-git-utils', exports='env')
SConscript('calsol-fw-libs/SConscript-openocd-utils', exports='env')
SConscript('calsol-fw-libs/SConscript-simplify', exports='env')

SConscript('mbed-scons/SConscript-env-gcc-arm', exports='env', duplicate=0)
SConscript('mbed-scons/SConscript-mbed', exports='env',duplicate=0)

##
## Submodule checks
##
env.CheckSubmoduleVersion()

AddOption(
  '--buildproto',
  action='store_true',
  help='Recursively build common-proto submodule, helpful when developing proto definitions.'
)
if GetOption('buildproto'):
  SConscript('common-proto/SConstruct')

###
### Build Variables
###
gitversion = env.GetGitVersion()
if gitversion is None:
  warnings.warn("Unable to get description of current Git HEAD, GITVERSION defaults to 'unknown'")
  gitversion = "unknown"

# Build wrapper for firmware that automatically builds the .elf, .bin, and dumps debugging data
def calsol_fw(env, target, srcs, includes, libs=[]):
  prog = env.Program(target, srcs,
      CPPDEFINES=env['CPPDEFINES'] + ['GITVERSION=\\"%s\\"' % gitversion],
      CPPPATH=env['CPPPATH'] + includes,
      LIBS=libs + env['LIBS']
  )
  flash = env.Flash('flash-' + target, prog)
  bin = env.Binary(prog)
  size = env.SymbolsSize(prog)
  dump = env.Objdump(prog)
  return (prog[0], flash)
env.AddMethod(calsol_fw, "CalSolFW")

###
### Platform-specific build targets for mbed libraries
###
# TODO: perhaps move environments into SConscript, but nested variant_dirs
# don't seem to work
env.Append(CPPDEFINES={'MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE': 115200})

env_orig = env

env = env_orig.Clone(FLASHINTERFACE='interface/cmsis-dap.cfg', FLASHTARGETCFG='lpc1549_openocd.cfg')
SConscript('SConscript-env-lpc1549', variant_dir='build/lpc1549', exports='env', duplicate=0)
SConscript('calsol-fw-libs/SConscript', variant_dir='build/lpc1549-libs', exports='env', duplicate=0)
env_lpc1549 = env

env = env_orig.Clone(FLASHINTERFACE='interface/stlink-v2-1.cfg', FLASHTARGETCFG='target/stm32f3x.cfg')
SConscript('SConscript-env-stm32f303', variant_dir='build/stm32f303', exports='env', duplicate=0)
SConscript('calsol-fw-libs/SConscript', variant_dir='build/stm32f303-libs', exports='env', duplicate=0)
env_stm32f303 = env

###
### Actual build targets here
###
all_targets = SConscript('SConscript', variant_dir='build', 
    exports=['env_lpc1549', 'env_stm32f303'], duplicate=0)

###
### Convenience targets
###
AddOption(
  '--list',
  action='store_true',
  help='List top-level targets.'
)

top_targets = []

for target in all_targets:
  name = os.path.splitext(os.path.basename(str(target[0])))[0]
  Default(target[0])
  top_targets.append(env.Alias(name, target[0]))
  top_targets.append(env.Alias('flash-' + name, target[1]))

if GetOption('list'):
  print('Top-level targets are: %s' % [str(x[0]) for x in top_targets])
  Exit(0)
