import time
from kivy.app import App
from kivy.uix.label import Label
from kivy.clock import Clock
from kivy.uix.button import Button
from kivy.config import Config
from kivy.storage.jsonstore import JsonStore
from kivy.core.audio import SoundLoader
from kivy.uix.boxlayout import BoxLayout

store = JsonStore('./data.json')


class setup:
    def __init__(self):
        data = []
        for num in range(1,int(self.io("Скільки уроків?")) + 1):
            data.append(str(self.io(str(num) + " уроки (в форматі 8:00)") ) )
        
        store.put("data", Lessons = data)
        print(store.get("data"))

    def io(self,q):
        a = input(str(q) + " ")
        return a

    def clear(self):
        store.clear()
        TimeApp().stop()


class IncrediblyCrudeClock(Label):
    def update(self, *args):
        self.text = time.asctime()
        #print(TimeApp.fl_alarm," in fl")
        #print(time.asctime().split()[3][:-3], "in time")
        if TimeApp.fl_alarm != time.asctime().split()[3][:-3]: TimeApp().parse_time()


class TimeApp(App):
    fl_alarm = "0"

    def parse_time(self):
        for num in store.get("data").get("Lessons"):
            if num == time.asctime().split()[3][:-3]:
                TimeApp.fl_alarm = num
                #print(num," out fl")
                self.alarm()
                break
  
    def alarm(self,*args):
        sound = SoundLoader.load('./alarm.wav')
        #sound.volume = 0.5
        sound.play()

    def build(self):
        self.title = "Lessons alarm"
        crudeclock = IncrediblyCrudeClock()
        Clock.schedule_interval(crudeclock.update, 0.2)
        crudeclock.set_top = True

        bl = BoxLayout(orientation="vertical", padding=20)

        bl.add_widget(crudeclock)
        bl.add_widget(Button(text="Clear settings", on_press=setup.clear))
        bl.add_widget(Button(text="ALARM!", on_press=TimeApp().alarm))
        return bl



if __name__ == "__main__":
    print(store.count())
    if store.count() == 0: setup()
    #TimeApp().alarm()
    TimeApp().run()