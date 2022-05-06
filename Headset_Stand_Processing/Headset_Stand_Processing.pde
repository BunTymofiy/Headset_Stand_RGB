int speed = 9600;  //<>//
String portName;
String[] patten_list = {"rainbow", "confetti", "sinelon", "juggle", "bpm", "colorScroll"};
int my_loop = 0;

import processing.serial.*;
import controlP5.*;
ControlP5 cp5;
Serial serial;

void setup() {
  size(500, 250);
  cp5 = new ControlP5(this);
  cp5.addButton("refresh").setPosition(110, 0).linebreak();
  cp5.addButton("open_Port").linebreak();
  cp5.addButton("close_Port").linebreak();
  cp5.addScrollableList("comlist").close();
  cp5.get(ScrollableList.class, "comlist").addItems(Serial.list());
  cp5.addScrollableList("pattern_list").close().setPosition(10, 120).hide();
  cp5.addSlider("Brightness").setPosition(130, 120).setSize(100, 20).setRange(0, 200).hide().setValue(100);
  cp5.addSlider("Speed").setPosition(130, 145).setSize(100, 20).setRange(100, 1).hide().setValue(100);
  cp5.addSlider("FPS").setPosition(130, 170).setSize(100, 20).setRange(1, 240).hide().setValue(120);
  cp5.addSlider("Loop_speed").setPosition(130, 200).setSize(100, 20).setRange(100, 1).hide().setValue(10);
  cp5.get(ScrollableList.class, "pattern_list").setItems(patten_list);
  cp5.addToggle("loop_patterns")
    .setPosition(300, 120)
    .setSize(50, 20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
    .hide()
    ;
}

void refresh() {
  cp5.get(ScrollableList.class, "comlist").setItems(Serial.list());
}
void comlist(int n) {
  portName = Serial.list()[n];
}
void open_Port() {
  serial = new Serial(this, portName, speed);
  cp5.get(ScrollableList.class, "pattern_list").show();
  cp5.get(Slider.class, "Brightness").show();
  cp5.get(Slider.class, "Speed").show();
  cp5.get(Slider.class, "FPS").show();
  cp5.get(Toggle.class, "loop_patterns").show();
  if (my_loop == 1) cp5.get(Slider.class, "Loop_speed").show();
}
void close_Port() {
  serial.stop();
  cp5.get(ScrollableList.class, "pattern_list").hide();
  cp5.get(Slider.class, "Brightness").hide();
  cp5.get(Slider.class, "Speed").hide();
  cp5.get(Slider.class, "FPS").hide();
  cp5.get(Toggle.class, "loop_patterns").hide();
  cp5.get(Slider.class, "Loop_speed").hide();
}
void pattern_list(int n) {
  String str = "0,";
  str += n;
  str += ";";
  if (serial != null) serial.write(str);
}
void Brightness(float brightness) {
  String str = "1,";
  str += brightness;
  str += ";";
  if (serial != null) serial.write(str);
}
void Speed(int speed) {
  String str = "2,";
  str += speed;
  str += ";";
  if (serial != null) serial.write(str);
}
void FPS(float fps) {
  String str = "3,";
  str += fps;
  str += ";";
  if (serial != null) serial.write(str);
}
void loop_patterns(boolean loop) {
  if (loop) my_loop = 1;
  if (!loop) my_loop = 0;
  println(my_loop);
  String str = "4,";
  str += my_loop;
  str += ";";
  if (serial != null) serial.write(str);
  if (my_loop == 1) {
    cp5.get(ScrollableList.class, "pattern_list").lock(); //<>//
    cp5.get(Slider.class, "Loop_speed").show(); //<>//
  } else {
    cp5.get(ScrollableList.class, "pattern_list").unlock();
    cp5.get(Slider.class, "Loop_speed").hide();
  }
}
void Loop_speed(int speed){
  String str = "5,";
  str += speed;
  str += ";";
  if (serial != null) serial.write(str);
}
void draw() {
  background(120);
  if (serial != null) {
    if (serial.available() > 0) {
      String str = serial.readStringUntil('\n');
      if (str != null) {
        str = str.trim();
        String data[] = str.split(",");
        int key = int(data[0]);
        switch(key)
        {
        case 0:
          //cp5.get(Slider.class, "Brightness").setValue(float(data[1]));
          break;
        case 1:
          //cp5.get(Slider.class, "Speed").setValue(int(data[1]));
          break;
        case 2:
          //cp5.get(Slider.class, "FPS").setValue(float(data[1]));
          break;
        case 3:
          //cp5.get(Toggle.class, "loop_patterns").setValue(boolean(data[1]));
          break;
        case 4:
          //cp5.get(Slider.class, "Loop_speed").setValue(int(data[1]));
          break;
        case 5:
          print(data[1]);
          break;
        }
      }
    }
  }
}
