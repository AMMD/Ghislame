env = Environment(CC='gcc',LIBS=['pthread','m','asound', 'liblo'],LIBPATH=['/usr/local/lib'])
env.ParseConfig('fltk-config --libs --cxxflags --ldflags')
env.Program('ghislame',"src/ghislame.cxx")
