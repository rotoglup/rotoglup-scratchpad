import time

if "reload_count" in globals():
  global reload_count
  reload_count += 1
else:
  reload_count = 0


def main():
  import application_classes
  i = application_classes.SomeClass()
  
  while 1:
    import application_functions
    print reload_count, application_functions.helper_function(), application_functions.CONSTANT, i.do_something()
    time.sleep(1.0)
