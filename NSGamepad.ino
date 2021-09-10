/* Teensy LC/3 becomes a USB NS Gamepad

   Select "NS Gamepad" from the "Tools > USB Type" menu
*/
/*
 * MIT License
 * 
 * Copyright (c) 2020 gdsports625@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
int usb_nsgamepad_send(void);
#ifdef __cplusplus
}
#endif

// Teensy 3.x / Teensy LC have the LED on pin 13
const int ledPin = 13;

void setup() {
  // you can print to the Serial1 port while the NSGamepad is active!
  Serial1.begin(115200);
  Serial1.println("NSGamepad setup");

  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  
  // Sends a clean HID report to the host.
  NSGamepad.begin();
}


unsigned long _btnDelay = 75;
unsigned long _delay = 100;

void left(int times=1) {
  dpad(NSGAMEPAD_DPAD_LEFT, times);
}

void right(int times=1) {
  dpad(NSGAMEPAD_DPAD_RIGHT, times);
}

void down(int times=1) {
  dpad(NSGAMEPAD_DPAD_DOWN, times);
}

void up(int times=1) {
  dpad(NSGAMEPAD_DPAD_UP, times);
}

void dpad(int8_t d, int times) {
  for (int i=0; i<times; i++) {
    dpad(d);
  }
}

void write() {
  while (usb_nsgamepad_send() != 0) { }
}

void dpad(int8_t d) {
  digitalWrite(ledPin, HIGH);
  NSGamepad.dPad(d);
  write();
  delay(_btnDelay);
  NSGamepad.dPad(NSGAMEPAD_DPAD_CENTERED);
  write();
  digitalWrite(ledPin, LOW);
  delay(_delay);
}

void btnPress(uint8_t btn, int times) {
  for (int i=0; i<times; i++) {
    btnPress(btn);
  }
}

void btnPress(uint8_t btn) {
  digitalWrite(ledPin, HIGH);
  NSGamepad.press(btn);
  write();
  delay(_btnDelay);
  NSGamepad.release(btn);
  write();
  digitalWrite(ledPin, LOW);
  delay(_delay);
}

void btnA(int times=1) {
  btnPress(NSButton_A, times);
}

void btnB(int times=1) {
  btnPress(NSButton_B, times);
}

void btnX(int times=1) {
  btnPress(NSButton_X, times);
}

void btnY(int times=1) {
  btnPress(NSButton_Y, times);
}

void wait(unsigned long ms=150) {
  delay(ms);
}

void select(unsigned long waitMS=150) {
  btnA();
  wait(waitMS);
}

void reincarnate(int charIndex, int srLineItem, bool generic, bool witch, bool maxEvilities=false) {
  if (charIndex > 1) {
    down(charIndex - 1); // scroll to character
  }
  select(); // select character
  down(srLineItem - 1); // scroll to Super Reincarnate

  select(); // select SR

  if (generic) {
    wait();
    down(); // select highest tier generic
    select(); // confirm
  }

  up(); // select Genius

  select(); // skip capabilities

  if (witch) {
    select(); // select element (fire)
  }

  if (generic) {
    select(); // select personality
  }

  delay(150);
  select(); // select color
  select(); // why???

  delay(150);
  left(); // highlight yes
  delay(300);
  select(); // select
  delay(300);

  if (maxEvilities) {
    select(); // accept max evility notice
    delay(100);
  }

  select(); // accept evility/extracts
  delay(300);
  btnB(); // go back to list
  delay(300);
}

void reincarnateDLC(int index, bool maxEvilities=false) {
  reincarnate(index, 3, false, false, maxEvilities);
}

void reincarnateUnique(int index, bool maxEvilities=false) {
  reincarnate(index, 4, false, false, maxEvilities);
}

void reincarnateGeneric(int index, bool witch=false, bool maxEvilities=false) {
  reincarnate(index, 5, true, witch, maxEvilities);
}

void reincarnateMao(int index) {
  reincarnateDLC(index, true);
}

void reincarnateZed(int index) {
  reincarnateUnique(index, true);
}

void reincarnateWarLady(int index, bool maxEvilities=false) {
  reincarnateGeneric(index, false, maxEvilities);
}

void reincarnateWitch(int index, bool maxEvilities=false) {
  reincarnate(index, 5, true, true, maxEvilities);
}

void doBills() {
  passBill(10);
  passBill(11);
}

void passBill(int lineItem) {
  select();
  if (lineItem > 1) {
    down(lineItem - 1);
  }
  select();
  delay(5000); // wait for scene to load
  btnX(); // menu
  select(); // vote
  delay(1500); // wait
  btnX(); // skip
  delay(7500); // wait for success
  select(); // accept bill pass
  delay(10000); // wait for loading screen
  select(); // re-enter menu
  delay(250);
}

void loop() {
  delay(1000);
  btnA();
  delay(2500);
  btnA();

  doBills();

  delay(2000);

  reincarnateWarLady(1);
  reincarnateWarLady(2);
  reincarnateWarLady(3);
  reincarnateWarLady(4);
  reincarnateWarLady(5);
  reincarnateWarLady(6);
  reincarnateWarLady(7);
  reincarnateWarLady(8);
  reincarnateWarLady(9);

  reincarnateMao(10);


  delay(30000);

//  btnPress(NSButton_A);
  delay(_delay);
}
