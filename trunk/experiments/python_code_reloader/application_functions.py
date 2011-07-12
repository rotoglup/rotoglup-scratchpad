if "reload_count" in globals():
  global reload_count
  reload_count += 1
else:
  reload_count = 0

# constants reload fine
CONSTANT = reload_count

# functions reload fine
def helper_function():
  return reload_count, id(globals())
