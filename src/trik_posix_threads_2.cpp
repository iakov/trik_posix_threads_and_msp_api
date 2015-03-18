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
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	connect_USBMSP();


	disconnect_USBMSP();

	return 0;
}
