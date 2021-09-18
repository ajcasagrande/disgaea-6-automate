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

#define NSButton_ZL NSButton_LeftThrottle
#define NSButton_ZR NSButton_RightThrottle

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
    btnPress(NSButton_ZR);
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

void reincarnateGeneric(int index, bool maxEvilities=false) {
  reincarnate(index, 5, true, false, maxEvilities);
}

void reincarnateWitch(int index, bool maxEvilities=false) {
  reincarnate(index, 5, true, true, maxEvilities);
}

void doBills() { // note: these numbers are based on a GENERIC as first !
  passBill(10); // triple-exp
  passBill(11); // welcome-party
  // passBill(10); // paise my performance
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

void quickAccess(uint8_t trigger, uint8_t stickX, uint8_t stickY) {
  btnHold(trigger);
  delay(500);
  leftStick(stickX, stickY);
  delay(200);
  leftStick(STICK_CENTER, STICK_CENTER);
  delay(50);
  btnRelease(trigger);
  delay(1000);
}


void quickAccessLeft(uint8_t stickX, uint8_t stickY) {
  quickAccess(NSButton_ZL, stickX, stickY);
}

void quickAccessRight(uint8_t stickX, uint8_t stickY) {
  quickAccess(NSButton_ZR, stickX, stickY);
}

void quickAccessStages() {
  quickAccessRight(STICK_CENTER, STICK_MIN);
}

void quickAccessDarkAssembly() {
  quickAccessLeft(STICK_MIN, STICK_MIN);
}

void setup() {
  // you can print to the Serial1 port while the NSGamepad is active!
  Serial1.begin(115200);
  Serial1.println("NSGamepad setup");

  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  
  // Sends a clean HID report to the host.
  NSGamepad.begin();

  write();
  delay(1000);
  // btnA();
  right();
  write();
  delay(2500);
  write();
  // btnB();
}

void gotoBellOfBlessing() {
  quickAccessStages();
  delay(2000);
  right(); // just to activate the controller
  btnPress(NSButton_ZR);
  delay(100);
  btnPress(NSButton_ZR);
  delay(100);
  right(); // just to activate the controller
  up(3); // peaceful world
  right(); // just to activate the controller
  select(); // for some reason first one doesn't work
  delay(1000);
  right(); // just to activate the controller
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
}

void reincarnateAll() {
  reincarnateGeneric(1, true); // jinx
  reincarnateGeneric(2); // smithereen
  reincarnateGeneric(3, true); // ulyses
  reincarnateGeneric(4); // trinket
  reincarnateGeneric(5, true); // yuina
  reincarnateGeneric(6); // made in japan
  reincarnateGeneric(7); // didi
  reincarnateGeneric(8); // gregory
  reincarnateGeneric(9); // ai

  reincarnateDLC(10, true); // mao
  reincarnateUnique(11, true); // zed

  reincarnateGeneric(12, true); // hiroko

  reincarnateUnique(13, true); // beiko
  reincarnateUnique(14, true); // majorlene
  reincarnateUnique(15, true); // melodia

  reincarnateDLC(16, true); // killia
  reincarnateDLC(17); // asagi
  reincarnateDLC(18); // adell
  
  reincarnateGeneric(19, true); // spilt milk
  reincarnateWitch(20, true); // mizuki

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
}

void reincarnateAll2() {
  reincarnateGeneric(1, true); // ulyses
  reincarnateDLC(2, true); // desco
  reincarnateDLC(3, true); // mao
  reincarnateGeneric(4, true); // smithereen
  reincarnateUnique(5, true); // majorlene
  reincarnateUnique(6, true); // beiko
  reincarnateDLC(7, true); // adell
  reincarnateGeneric(8, true); // mizuki (as war lady)
  reincarnateDLC(9, true); // killia
  reincarnateDLC(10, true); // valvotorez
  reincarnateDLC(11, true); // fuka
  reincarnateUnique(12, true); // zed
  reincarnateGeneric(13, true); // hiroko
  reincarnateUnique(14, true); // melodia
  reincarnateGeneric(15, true); // spilt milk
  reincarnateGeneric(16, true); // yuina

  reincarnateGeneric(17, true); // yuri
  reincarnateGeneric(18, true); // jinx
  reincarnateGeneric(19, true); // trinket
  reincarnateGeneric(20, true); // ai
  reincarnateGeneric(21, true); // made in japan
  reincarnateGeneric(22, true); // didi
  reincarnateGeneric(23, true); // thigh highs
  reincarnateGeneric(24, true); // eliza
  reincarnateGeneric(25, true); // annie
  reincarnateGeneric(26, true); // eva
  reincarnateGeneric(27, true); // pogo
}

void reincarnationLoop() {
  quickAccessDarkAssembly();
  select(); // enter assembly
  delay(500);

  doBills();
  delay(2000);

  reincarnateAll2();

  btnB(2); // leave dark assembly
  delay(6500); // wait for loading screen

  gotoBellOfBlessing();

  // loop!
}

void juiceBarLoop() {
  select(); // select char
  delay(500);
  down(); // go down to mana

  for (int i=0; i <255; i++) {
    right(); // increase mana
    // delay(100);
    select(); // choose
    // delay(100);
    select(); // accept preview
    delay(50);
    up(); // select yes
    // delay(100);
    select(); // drink
    delay(50);
  }

  btnB(); // go back
  delay(150);
  down();
  delay(150);
}

unsigned long began = millis();
unsigned long maxMillis = 22 * 51 * 1000; // 22 seconds * 51 innocents * 1000 ms/s

void bossKillLoop() {
  if (millis() - began <= maxMillis) {
    btnPress_internal(NSButton_A, true);
    delay(_btnDelay);
    // btnPress_internal(NSButton_A, false);
  }
}

void loop() {
  // bossKillLoop();
  reincarnationLoop();
  // juiceBarLoop();
}
