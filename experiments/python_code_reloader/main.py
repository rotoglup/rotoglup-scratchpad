# main module for code reloader management
# the application supposed to be reloaded/reloadable is in application.py

import os
import thread
import threading
import time
import types

import application

nthread = threading.Thread(target = application.main)
nthread.start()
#nthread.join()

# helper functions

def isReservedName(name):
  return name.startswith("__")

def rebindFunction(function, globals_):
  newFunction = types.FunctionType(function.func_code, globals_, function.func_name, function.func_defaults)
  newFunction.__doc__= function.__doc__
  newFunction.__dict__.update(function.__dict__)
  return newFunction

# gather reloadable modules

reloadable_folders = [ os.path.dirname(__file__) ]
reloadable_modules = []

import sys
for moduleName, module in sorted( sys.modules.items() ):
  
  if module is None:
    continue
  
  if isReservedName(moduleName):
    continue
  
  module_file = getattr(module, "__file__", "")
  reloadable = False
  for path in reloadable_folders:
    if module_file.startswith(path):  # TODO a real test
      reloadable = True
  if not reloadable:
    continue
  
  print moduleName, module.__dict__.keys()
  reloadable_modules.append( module )

# reload function

def rtgu_reload(module):
  
  prev_globals = dict( module.__dict__ )
  
  reload(module)
  
  new_globals = module.__dict__
  
  for name, value in prev_globals.iteritems():
  
    if isReservedName(name):
      continue
    
    new_value = new_globals.get(name, None)
    if new_value is None:
      # has been deleted, let it dangling
      continue
    
    if type(value) in [ types.ClassType, types.ObjectType ]:
      
      # TODO check new_value type
      
      print "* ", name
      for attrname in dir(value):
        
        attrvalue = getattr(new_value, attrname)
        if not callable(attrvalue):
          continue
        
        new_attrvalue = rebindFunction(attrvalue, new_globals)
        
        setattr(value, attrname, new_attrvalue)
        print attrname, type(attrvalue)
        
  print prev_globals.get("reload_count", None), module.__dict__.get("reload_count", None)

#

time.sleep(1.0)

print id(application.__dict__)
rtgu_reload(application)
print id(application.__dict__)
rtgu_reload(application)
print id(application.__dict__), id( sys.modules["application_functions"] )
for module in reloadable_modules:
  rtgu_reload(module)
print id(application.__dict__), id( sys.modules["application_functions"] )
for module in reloadable_modules:
  rtgu_reload(module)
print id(application.__dict__), id( sys.modules["application_functions"] )
