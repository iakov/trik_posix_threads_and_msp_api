/*
 * access_devices.cpp
 *
 *  Created on: Mar 18, 2015
 *      Author: Rostislav Varzar
 */

#include "usbMSP430Defines.h"
#include "usbMSP430Interface.h"
#include "access_devices.h"

/// Start 1 and 2 motors
void start_motors_12()
{
	char s1[MAX_STRING_LENGTH];	// Temp string

	makeWriteRegPacket(s1, MOTOR1, MMPER, 0x5000);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, MOTOR1, MMDUT, 0x4000);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, MOTOR1, MMCTL, MOT_ENABLE + MOT_POWER);
	sendUSBPacket(s1, s1);

	makeWriteRegPacket(s1, MOTOR2, MMPER, 0x5000);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, MOTOR2, MMDUT, 0x4000);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, MOTOR2, MMCTL, MOT_ENABLE + MOT_POWER);
	sendUSBPacket(s1, s1);
}

/// Stop 1 and 2 motors
void stop_motors_12()
{
	char s1[MAX_STRING_LENGTH];	// Temp string

	makeWriteRegPacket(s1, MOTOR1, MMCTL, MOT_ENABLE + MOT_BRAKE);
	sendUSBPacket(s1, s1);

	makeWriteRegPacket(s1, MOTOR2, MMCTL, MOT_ENABLE + MOT_BRAKE);
	sendUSBPacket(s1, s1);
}

