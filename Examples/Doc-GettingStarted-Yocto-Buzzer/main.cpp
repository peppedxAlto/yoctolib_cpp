/*********************************************************************
 *
 *  $Id: main.cpp 42524 2020-11-17 10:03:41Z seb $
 *
 *  An example that show how to use a  Yocto-Buzzer
 *
 *  You can find more information on our web site:
 *   Yocto-Buzzer documentation:
 *      https://www.yoctopuce.com/EN/products/yocto-buzzer/doc.html
 *   C++ API Reference:
 *      https://www.yoctopuce.com/EN/doc/reference/yoctolib-cpp-EN.html
 *
 *********************************************************************/

#include "yocto_api.h"
#include "yocto_buzzer.h"
#include "yocto_anbutton.h"
#include "yocto_led.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>

using namespace std;

static void usage(void)
{
  cout << "usage: demo <serial_number> " << endl;
  cout << "       demo any  (use any discovered device)" << endl;
  u64 now = YAPI::GetTickCount();
  while (YAPI::GetTickCount() - now < 3000) {
    // wait 3 sec to show the message
  }
  exit(1);
}

int main(int argc, const char * argv[])
{
  string  errmsg;
  string  target, serial;
  YBuzzer  *buz;
  YLed    *led1, *led2 , *led;
  YAnButton *button1, *button2;
  int    i;
  int frequency;

  if (argc < 2) {
    usage();
  }
  target = (string) argv[1];

  // Setup the API to use local USB devices
  if (YAPI::RegisterHub("usb", errmsg) != YAPI::SUCCESS) {
    cerr << "RegisterHub error: " << errmsg << endl;
    return 1;
  }

  if (target == "any") {
    buz = YBuzzer::FirstBuzzer();
    if (buz == NULL) {
      cout << "No module connected (check USB cable)" << endl;
      return 1;
    }
  } else {
    buz = YBuzzer::FindBuzzer(target + ".buzzer");
  }

  if (!buz->isOnline()) {
    cout << "Module not connected (check identification and USB cable)" << endl;
    return 1;
  }
  frequency = 1000;
  serial    = buz->get_module()->get_serialNumber();
  led1      = YLed::FindLed(serial + ".led1");
  led2      = YLed::FindLed(serial + ".led2");
  button1   = YAnButton::FindAnButton(serial + ".anButton1");
  button2   = YAnButton::FindAnButton(serial + ".anButton2");

  cout << "press a test button or hit Ctrl-C" << endl;
  while (1) {
    if (!button1->isOnline()) {
      cout << "Module not connected (check identification and USB cable)" << endl;
      break;
    }
    int b1 = button1->isPressed();
    int b2 = button2->isPressed();
    if (b1 || b2 ) {
      if (b1) {
        led = led1;
        frequency = 1500;
      } else {
        led = led2;
        frequency = 500;
      }
      led->set_power(YLed::POWER_ON);
      led->set_luminosity(100);
      led->set_blinking(YLed::BLINKING_PANIC);
      for (i = 0; i < 5; i++) {
        // this could be done using a sequence as well
        buz->set_frequency(frequency);
        buz->freqMove(2 * frequency, 250);
        YAPI::Sleep(250, errmsg);
      }
      buz->set_frequency(0);
      led->set_power(YLed::POWER_OFF);
    }
  }
  YAPI::FreeAPI();
}

