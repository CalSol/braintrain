Import('env_lpc1549')

env = env_lpc1549.Clone()

env.Append(CCFLAGS=['-Wall', '-Werror'])

env.CalSolFW('brain',
  srcs=Glob('src/*.cpp'),
  includes=['src']
)
