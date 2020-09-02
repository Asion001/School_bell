import time
from kivy.app import App
from kivy.uix.label import Label
from kivy.clock import Clock
from kivy.uix.button import Button
from kivy.config import Config
from kivy.storage.jsonstore import JsonStore
from kivy.core.audio import SoundLoader

sound = SoundLoader.load('./alarm.wav')
store = JsonStore('./data.json')


class setup():
    def __init__(self):
        data = []
        for num in range(1,int(self.io("Скільки уроків?")) + 1):
            data.append(str(self.io(str(num) + " урок (в форматі 8:00)") ) )
        
        store.put("data", Lessons = data)
        print(store.get("data"))

    def io(self,q):
        a = input(str(q) + " ")
        return a


class IncrediblyCrudeClock(Label):
    def update(self, *args):
        self.text = time.asctime()
        print(time.asctime())

class TimeApp(App):
    def build(self):
        self.title = "Lessons alarm"
        crudeclock = IncrediblyCrudeClock()
        Clock.schedule_interval(crudeclock.update, 0.2)
        crudeclock.set_top = True


        return crudeclock

if __name__ == "__main__":
    print(store.count())
    if store.count() == 0: setup()

    TimeApp().run()