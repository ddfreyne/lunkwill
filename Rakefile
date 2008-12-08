TARGET_BIN  = 'lunkwill_test'
TARGET_LIB  = 'lunkwill.dylib'

CFLAGS      = '--std=c99 -W -Wall -Iinclude'
LDFLAGS_BIN = ''
LDFLAGS_LIB = ''

CC          = 'gcc'

### stuff you don't need to care about

require 'rake/clean'

SRCS = FileList['src/**/*.c']
OBJS = SRCS.ext('o')

CLEAN.include(OBJS)
CLOBBER.include(TARGET_BIN, TARGET_LIB)

### tasks

task :default => [ :build, :run ]

task :build => [ TARGET_BIN, TARGET_LIB ]

task :run => [ TARGET_BIN ] do
  sh "echo ; ./#{TARGET_BIN}"
end

### rules

rule '.o' => '.c' do |t|
  sh "#{CC} -c #{CFLAGS} -o #{t.name} #{t.source}"
end

file TARGET_BIN => OBJS do
  sh "#{CC} #{CFLAGS} #{LDFLAGS_BIN} -o #{TARGET_BIN} #{OBJS}"
end

file TARGET_LIB => OBJS do
  sh "#{CC} #{CFLAGS} #{LDFLAGS_LIB} -dynamiclib -o #{TARGET_LIB} #{OBJS}"
end
