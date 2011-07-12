class Dog(object):
    def talk(self):
        print "bark"
        
    def talk2(self):
      print "blou"
      
class Another(object):
  
  def talk(self):
    print "flah"

dog1 = Dog()
dog1.talk()  # --> Bark

def cat_talk(self):
    print "Meow"

#Dog.talk = cat_talk
setattr(Dog, "talk", lambda x: Another.talk(x))
dog1.talk()  # --> Meow
