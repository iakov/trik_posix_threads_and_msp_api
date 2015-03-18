//============================================================================
// Name        : trik_posix_threads_2.cpp
// Author      : Rostislav Varzar
// Version     :
// Copyright   : Free license
// Description : C++, Ansi-style
//============================================================================

#include <iostream>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "usbMSP430Defines.h"
#include "usbMSP430Interface.h"
#include "access_devices.h"

using namespace std;



int main()
{
	// cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	connect_USBMSP();
	init_motors_12();
	init_sensors_123();
	start_motors_12();

	cout << "Sensor1 value: " << read_sensor_1() << endl;
	cout << "Sensor2 value: " << read_sensor_2() << endl;
	cout << "Sensor3 value: " << read_sensor_3() << endl;



	stop_motors_12();
	disconnect_USBMSP();

	return 0;
}
