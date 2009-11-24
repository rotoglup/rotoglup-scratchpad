import os

GCC_PATH = 'F:\\temp\\MinGW\\bin\\gcc.exe'

#env = Environment(ENV = os.environ, tools = ['mingw'])
env = Environment(ENV = os.environ)

compiler = env.subst('$CC')
platform = env['PLATFORM']
print compiler, platform

if platform == "darwin":
  env.Append(CPPPATH = ["/usr/local/boost_1_37_0", "#../../../external", "#../../.."])
  env.Append(LIBPATH = [])
else:
  env.Append(CPPPATH = ["M:\\boost_1_37\\include\\boost-1_37", "#..\\..\\..\\external", "#..\\..\\.."])
  env.Append(LIBPATH = ["M:\\boost_1_37\\lib"])

#-------------------------------------------------------------------------------

if compiler == 'cl':

  CPPFLAG_DEBUG_PDB = "/Zi "
  OUTPUT_ASSEMBLY = "/FAs "
  OPTIMIZE_FLAGS = "/MT /O2 /Ob2 /Ot /Oi /Oy /GS- /arch:SSE2 /fp:fast /DNDEBUG /D_CRT_SECURE_NO_DEPRECATE /D_SECURE_SCL=0 "
  #OPTIMIZE_FLAGS = "/MTd /Od "
  
  env.Append(CPPFLAGS = "/EHsc /DMGOPENGL_STATIC_LIB /DMGOPENGL_DISABLE_AUTOLINK " + OPTIMIZE_FLAGS + CPPFLAG_DEBUG_PDB)
    
  # enable manifest embedding
  #env[ 'WINDOWS_INSERT_MANIFEST' ] = True     # needed to clean the .manifest file
  #env[ 'SHLINKCOM' ] = [ env[ 'SHLINKCOM' ], 'mt -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2' ]
  #env[ 'LINKCOM'   ] = [ env[ 'LINKCOM'   ], 'mt -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1' ]

elif compiler == 'gcc':

  print "** MinGW **"
  
  """
  Dynamic linking with libgcc_s_dw2-1.dll is necessary to throw  exceptions between different modules, such as between two DLLs or a
  DLL and an EXE.  Consequently, it is the default for all languages other than C. To disable this dynamic linking, use -static-libgcc.
  To enable this dynamic linking in C, use -shared-libgcc.
  """
  #?#env.Append(CPPFLAGS = "-static-libgcc")
  
  OPTIMIZE_FLAGS  = "-O3 -ffast-math -msse -msse2 -mfpmath=sse -DNDEBUG "
  # no avail on gcc 4.0.1 # OPTIMIZE_FLAGS += "-mtune=core2 "
  OPTIMIZE_FLAGS += "-fomit-frame-pointer "
  OPTIMIZE_FLAGS += "-fstrict-aliasing -fargument-noalias -fargument-noalias-global "
  OPTIMIZE_FLAGS += "-fcprop-registers "
  # no avail on gcc 4.0.1 # OPTIMIZE_FLAGS += "-fipa-cp -fipa-pta -fipa-pure-const -fipa-reference "
  OPTIMIZE_FLAGS += "-fschedule-insns2 -fsched2-use-superblocks "
  OPTIMIZE_FLAGS += "-fivopts "
  OPTIMIZE_FLAGS += "-ftree-loop-linear "
  OPTIMIZE_FLAGS += "-ftree-loop-im "
  OPTIMIZE_FLAGS += "-ftree-loop-ivcanon "
  
  OUTPUT_ASSEMBLY = "-fverbose-asm --pedantic -Wall "# + "-S "

  COMPILER_VERBOSE = "-v"
  env.Append(CPPFLAGS  = OPTIMIZE_FLAGS + OUTPUT_ASSEMBLY + "-DMGOPENGL_STATIC_LIB")
  
  LINKER_VERBOSE = "-Wl,--verbose"
  #?#env.Append(LINKFLAGS = "-Wl,-subsystem,windows ")      #!< allow to recognize Win32 libraries

#-------------------------------------------------------------------------------

FRAMEWORK = []

Export('env')

#

def make_program(main_name):

  version_env = env.Clone()

  # Enable debug information (SCons native way)
  
  version_env[ 'PDB' ] = "%s.pdb" % main_name # os.path.join( builddir, "%s.pdb" % BASE_NAME )
  
  # NOTE:: scons native PDB handling uses /Z7 switch, which causes debug information to be put in each '.obj'
  # this creates huge .obj files, better use /Zi to incrementally create the PDB, and remove redudent debug information
  # Scons doc (http://www.scons.org/doc/1.2.0/HTML/scons-man.html) indicates that is may hurt parallel build performance.
  
  version_env['CCPDBFLAGS']  = ['${(PDB and "/Fd%s_incremental.pdb /Zi" % File(PDB)) or ""}']
  version_env['PCHPDBFLAGS'] = ['${""}']                                        #!< removed the deprecated '/Yd' scons uses by default

  #

  DEFAULT_LIBS = [ "" ]
  
  if compiler == 'cl':
  
    DEFAULT_LIBS = ['user32.lib', 'gdi32.lib']
  
  elif compiler == 'gcc':
  
    if platform == "darwin":
      pass
    else:
      DEFAULT_LIBS = ['user32', 'gdi32', 'winmm']

  #
  
  framework = [ version_env.Object(name + "%s_framework" % main_name, name) for name in FRAMEWORK ]

  program = version_env.Program(main_name, [ main_name+'.cpp' ] + framework, LIBS=[]+DEFAULT_LIBS)
  
  return program

##

UnitTestPlusPlus = SConscript('#/../../../external/UnitTest++/SConscript')
print "UnitTestPlusPlus = %s" % str(UnitTestPlusPlus)

rtgu_image_io = SConscript('#/../../image_io/SConscript')
print "rtgu_image_io = %s" % str(rtgu_image_io)

##

def make_test(name, additional_libs=[]):
  
  version_env = env.Clone()

  # Enable debug information (SCons native way)
  
  version_env[ 'PDB' ] = "%s.pdb" % name # os.path.join( builddir, "%s.pdb" % BASE_NAME )
  
  # NOTE:: scons native PDB handling uses /Z7 switch, which causes debug information to be put in each '.obj'
  # this creates huge .obj files, better use /Zi to incrementally create the PDB, and remove redudent debug information
  # Scons doc (http://www.scons.org/doc/1.2.0/HTML/scons-man.html) indicates that is may hurt parallel build performance.
  
  version_env['CCPDBFLAGS']  = ['${(PDB and "/Fd%s_incremental.pdb /Zi" % File(PDB)) or ""}']
  version_env['PCHPDBFLAGS'] = ['${""}']                                        #!< removed the deprecated '/Yd' scons uses by default
  
  #

  DEFAULT_LIBS = []
  
  if compiler == 'cl':
  
    DEFAULT_LIBS = ['user32.lib', 'gdi32.lib']
  
  elif compiler == 'gcc':
  
    if platform == "darwin":
      pass
    else:
      DEFAULT_LIBS = ['user32', 'gdi32', 'winmm']

  sources = [ name+'.cpp' ] + FRAMEWORK + UnitTestPlusPlus + additional_libs

  program = version_env.Program(name, sources, LIBS=[] + DEFAULT_LIBS)
  
  """
  if compiler == 'cl':
  
    sources, headers = version_env.FindAllSourceFiles(program)
    
    version_env.MSVSProject(name + version_env['MSVSPROJECTSUFFIX'],
                    srcs = sources,  #[ name+'.cpp' ] + FRAMEWORK,
                    incs = headers,
                    buildtarget = program,
                    variant = "Release")
  """
  
#---
  
make_test('image_algorithm')
make_test('image_creation')
make_test('image_gil_internals')
make_test('image_io_gil_borrowed_image')
make_test('image_resize') #, additional_libs=rtgu_image_io)
if platform != "darwin":
  make_test('image_io', additional_libs=rtgu_image_io)
