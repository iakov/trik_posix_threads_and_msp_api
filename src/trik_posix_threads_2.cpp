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

using namespace std;

char s1[MAX_STRING_LENGTH];	// Temp string
int tmp1;

int main()
{
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	connect_USBMSP();

	makeWriteRegPacket(s1, MOTOR1, MMPER, 0x5000);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, MOTOR1, MMDUT, 0x3000);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, MOTOR1, MMCTL, MOT_ENABLE + MOT_POWER);
	sendUSBPacket(s1, s1);

	cin >> tmp1;

	makeWriteRegPacket(s1, MOTOR1, MMCTL, MOT_ENABLE + MOT_BRAKE);
	sendUSBPacket(s1, s1);



	disconnect_USBMSP();

	return 0;
}
