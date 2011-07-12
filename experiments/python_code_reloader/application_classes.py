if "reload_count" in globals():
  global reload_count
  reload_count += 1
else:
  reload_count = 0

if reload_count == 0:
  
  class SomeClass:
    
    def do_something(self):
      return -1

else:
  
  class SomeClass:
    
    def do_something(self):
      return reload_count
    
    def do_something_more(self):
      return None

  