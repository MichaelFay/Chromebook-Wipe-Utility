/*
  This program only runs ones. Therefor we do not have a functions in the loop
  There is no external led's on the Bad Bettle USB. If you have a device that supports LEDS it is recommend that you flash the LED to signifiy device is complete
  This application was insipred by the work that was done by Amplified IT labs (ttp://labs.amplifiedit.com/centipede). 
*/

//Required includes
#include <Arduino.h>
#include <Keyboard.h>
#include "config.h"

// Define Special Keys that will be used when powerwashing and re-enrolling
#define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT   0x82
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT    0x85
#define KEY_RIGHT_ALT  0x86
#define KEY_UP_ARROW   0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW    0xD7
#define KEY_BACKSPACE  0xB2
#define KEY_TAB        0xB3
#define KEY_ENTER 0xB0
#define KEY_ESC        0xB1
#define KEY_CAPS_LOCK  0xC1


#define SECOND_DELAY(x)  delay(1000*x)

static uint8_t __clock_prescaler = (CLKPR & (_BV(CLKPS0) | _BV(CLKPS1) | _BV(CLKPS2) | _BV(CLKPS3)));


void repeatKey(byte key, int num);
void enterCredentials() ;
void enrollDevice() ;
void configureWIFI();
void configurePrescaler();
void powerWash();
void flagOSVerifcation();

void loop() {
  // put your main code here, to run repeatedly:
}

void setup() {
  configurePrescaler();
  Keyboard.begin();

  SECOND_DELAY(10);

  if(HARD_RST){
    flagOSVerifcation();
    SECOND_DELAY(1);
  }
    

  if(ENROLL_DEVICE){
    configureWIFI();
    enterCredentials();
  }
  
}


void flagOSVerifcation(){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.write('d');
  Keyboard.release(KEY_LEFT_CTRL);

  while(true){
    Keyboard.write(KEY_LEFT_ARROW);
    SECOND_DELAY(1);
  }
}

void powerWash(){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.write('r');
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release(KEY_LEFT_ALT);
}

void repeatKey(byte key, int num) {
  for (int i = 0; i < num; i++) {
    Keyboard.write(key);
    SECOND_DELAY(1);
  }
}

void enterCredentials() {
  SECOND_DELAY(2);
  Keyboard.print(ENROLLMENT_USERNAME);
  SECOND_DELAY(1);
  Keyboard.write(KEY_ENTER);
  SECOND_DELAY(1);
  Keyboard.print(ENROLLMENT_PASSWORD);
  SECOND_DELAY(1.5);
  Keyboard.write(KEY_ENTER);
  SECOND_DELAY(1);
}

void enrollDevice() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.write('e');
  Keyboard.release(KEY_LEFT_ALT);
  Keyboard.release(KEY_LEFT_CTRL);
  SECOND_DELAY(.5);
}

void configureWIFI() {
  // TABs to enter Wifi Configuration. Change the number below to match if your devices require a different number of tabs to select the wifi configuration
  repeatKey(KEY_TAB, 3);
  Keyboard.write(KEY_ENTER);
  repeatKey(KEY_UP_ARROW, 2);

  Keyboard.write(KEY_ENTER);
  SECOND_DELAY(.5);

  // SSID
  Keyboard.print(WIFI_SSID);
  SECOND_DELAY(.5);

  // TAB
  Keyboard.write(KEY_TAB);
  SECOND_DELAY(.5);

  if (WIFI_SECURITY_TYPE == 0) {
  	repeatKey(KEY_TAB,2);
  } else {
  	repeatKey(KEY_DOWN_ARROW, WIFI_SECURITY_TYPE);

  	// TAB
  	Keyboard.write(KEY_TAB);
    SECOND_DELAY(.5);

  	// type wifi password
    Keyboard.print(WIFI_PASSWORD);
    
  }

  SECOND_DELAY(.5);
  // Enter
  Keyboard.write(KEY_ENTER);

  SECOND_DELAY(.5);

  // Delay 10 seconds to connect
  SECOND_DELAY(10);

  Keyboard.write(KEY_TAB);
  SECOND_DELAY(.5);

  Keyboard.write(KEY_ENTER);
  SECOND_DELAY(.5);

  enrollDevice();

  Keyboard.write(KEY_ENTER);
}

void configurePrescaler() {
	// Disable interrupts.
    uint8_t oldSREG = SREG;
    cli();

    // Enable change.
    CLKPR = _BV(CLKPCE); // write the CLKPCE bit to one and all the other to zero

    // Change clock division.
    CLKPR = 0x0; // write the CLKPS0..3 bits while writing the CLKPE bit to zero

    // Copy for fast access.
    __clock_prescaler = 0x0;

    // Recopy interrupt register.
    SREG = oldSREG;
}