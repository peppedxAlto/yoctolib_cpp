/*********************************************************************
 *
 *  $Id: main.cpp 44692 2021-04-26 12:17:53Z seb $
 *
 *  An example that show how to use a  Yocto-MaxiMicroVolt-Rx
 *
 *  You can find more information on our web site:
 *   Yocto-MaxiMicroVolt-Rx documentation:
 *      https://www.yoctopuce.com/EN/products/yocto-maximicrovolt-rx/doc.html
 *   C++ API Reference:
 *      https://www.yoctopuce.com/EN/doc/reference/yoctolib-cpp-EN.html
 *
 *********************************************************************/

#include "yocto_api.h"
#include "yocto_genericsensor.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

static void usage(void)
{
  cout << "usage: demo <serial_number> " << endl;
  cout << "       demo <logical_name>" << endl;
  cout << "       demo any" << endl;
  u64 now = YAPI::GetTickCount();
  while (YAPI::GetTickCount() - now < 3000) {
    // wait 3 sec to show the message
  }
  exit(1);
}

int main(int argc, const char * argv[])
{
  string errmsg, target;
  YGenericSensor *sensor;

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
    sensor = YGenericSensor::FirstGenericSensor();
    if (sensor == NULL) {
      cout << "No module connected (check USB cable)" << endl;
      return 1;
    }
  } else {
    sensor = YGenericSensor::FindGenericSensor(target + ".genericSensor1");
  }

  YGenericSensor *s1 = YGenericSensor::FindGenericSensor(
                         sensor->get_module()->get_serialNumber() +
                         ".genericSensor1");
  YGenericSensor *s2 = YGenericSensor::FindGenericSensor(
                         sensor->get_module()->get_serialNumber() +
                         ".genericSensor2");

  string unitSensor1, unitSensor2;

  if (s1->isOnline()) unitSensor1 = s1->get_unit();
  if (s2->isOnline()) unitSensor2 = s2->get_unit();

  while (s1->isOnline() && s2->isOnline())  {
    double value = s1->get_currentValue();
    cout << "Channel 1 : " << s1->get_currentValue() << unitSensor1;
    value = s2->get_currentValue();
    cout << "  Channel 2 : " << s2->get_currentValue() << unitSensor2;
    cout << "  (press Ctrl-C to exit)" << endl;
    YAPI::Sleep(1000, errmsg);
  };
  YAPI::FreeAPI();
  cout << "Module not connected (check identification and USB cable)";
  return 0;
}
