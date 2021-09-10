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

#define STICK_CENTER (128)
#define STICK_MIN (0)
#define STICK_MAX (255)

// Teensy 3.x / Teensy LC have the LED on pin 13
const int ledPin = 13;


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

void btnPress_internal(uint8_t btn, bool noDelay=false) {
  digitalWrite(ledPin, HIGH);
  NSGamepad.press(btn);
  write();
  delay(_btnDelay);
  NSGamepad.release(btn);
  write();
  digitalWrite(ledPin, LOW);
  if (!noDelay) {
    delay(_delay);
  }
}

void btnPress(uint8_t btn, int times=1) {
  for (int i=0; i<times; i++) {
    btnPress_internal(btn);
  }
}

void skipTo(int index, int skipAmount) {
  int amount = index-1;
  while (amount >= int(skipAmount / 2) + 1) {
    btnPress(NSButton_RightThrottle);
    delay(100);
    amount -= skipAmount;
  }
  if (amount > 0) {
    down(amount);
  } else if (amount < 0) {
    up(-amount);
  }
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

#define SKIP_SIZE_ASSEMBLY_CHARS (8)
#define SKIP_SIZE_ASSEMBLY_BILLS (8)
#define NO_SKIP (99999)

void reincarnate(int charIndex, int srLineItem, bool generic, bool witch, bool maxEvilities=false) {
  skipTo(charIndex, SKIP_SIZE_ASSEMBLY_CHARS); // scroll to character
  select(); // select character
  skipTo(srLineItem, SKIP_SIZE_ASSEMBLY_BILLS); // scroll to Super Reincarnate

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
    // delay(100);
  }

  btnPress_internal(NSButton_A, true); // accept evility/extracts
  btnPress_internal(NSButton_A, true);
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

void doBills() { // note: these numbers are based on war-lady as first!
  passBill(10); // triple-exp
  passBill(11); // welcome-party
}

void passBill(int lineItem) {
  select(); // just select first character
  skipTo(lineItem, SKIP_SIZE_ASSEMBLY_BILLS);
  select();
  delay(4000); // wait for scene to load
  btnX(); // menu
  select(); // vote
  delay(1500); // wait
  btnX(); // skip
  delay(7500); // wait for success
  select(); // accept bill pass
  delay(7000); // wait for loading screen
  select(); // re-enter menu
  delay(250);
}

void leftStick(uint8_t x, uint8_t y) {
  NSGamepad.leftXAxis(x);
  NSGamepad.leftYAxis(y);
  write();
}

void btnHold(uint8_t btn) {
  NSGamepad.press(btn);
  write();
  delay(200);
}

void btnRelease(uint8_t btn) {
  NSGamepad.release(btn);
  write();
  delay(200);
}


void quickAccessStages() {
    btnHold(NSButton_RightThrottle);
    delay(500);
    leftStick(STICK_CENTER, STICK_MIN);
    delay(200);
    leftStick(STICK_CENTER, STICK_CENTER);
    delay(50);
    btnRelease(NSButton_RightThrottle);
    delay(1000);
}


void quickAccessDarkAssembly() {
    btnHold(NSButton_LeftThrottle);
    delay(500);
    leftStick(STICK_MIN, STICK_MIN);
    delay(200);
    leftStick(STICK_CENTER, STICK_CENTER);
    delay(50);
    btnRelease(NSButton_LeftThrottle);
    delay(1000);
    select(); // enter assembly
    delay(500);
}

void setup() {
  // you can print to the Serial1 port while the NSGamepad is active!
  Serial1.begin(115200);
  Serial1.println("NSGamepad setup");

  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  
  // Sends a clean HID report to the host.
  NSGamepad.begin();

  delay(1000);
  btnA();
  delay(2500);
  btnB();
}

void loop() {
  quickAccessDarkAssembly();

  doBills();

  delay(2000);

  reincarnateWarLady(1); // jinx
  reincarnateWarLady(2); // smithereen
  reincarnateWarLady(3, true); // ulyses
  reincarnateWarLady(4); // trinket
  reincarnateWarLady(5); // yuina
  reincarnateWarLady(6); // made in japan
  reincarnateWarLady(7); // didi
  reincarnateWarLady(8); // gregory
  reincarnateWarLady(9); // ai

  reincarnateMao(10); // mao
  reincarnateZed(11); // zed

  reincarnateWarLady(12, true); // hiorki

  reincarnateUnique(13); //beiko
  reincarnateUnique(14); // majorlene
  reincarnateUnique(15); // melodia

  reincarnateDLC(16, true); // killia
  reincarnateDLC(17); // asagi 
  reincarnateDLC(18); // adell
  
  reincarnateGeneric(19, false, true); // spilt milk
  reincarnateWitch(20); // mizuki

  reincarnateUnique(21); // flonne
  reincarnateUnique(22); // misedor
  reincarnateDLC(23); // valvotorez
  reincarnateDLC(24); // pleinair
  reincarnateDLC(25); // fuka
  reincarnateUnique(26); // etna
  reincarnateGeneric(27); // yuri
  reincarnateDLC(28); // desco
  reincarnateDLC(29); // raspberyl
  reincarnateUnique(30); // ivar

  btnB(2); // leave dark assembly

  delay(6500); // wait for loading screen


  quickAccessStages();
  delay(2000);
  up(3); // peaceful world
  select();
  select(); // for some reason first one doesn't work
  delay(1000);
  up(); // bell of blessing
  select(); // select stage
  delay(500);
  select(); // select yes

  delay(30000); // wait for battle

  select(); // accept winnings
  delay(100);
  select(); // skip waiting for levels
  select(); // accept levels

  delay(5000); // wait to be back at menu

  btnB(3); // quit out

  delay(5000); // wait for back to main screen

  // loop!
}
