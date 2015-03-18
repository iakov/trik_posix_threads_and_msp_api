/*
 * access_devices.cpp
 *
 *  Created on: Mar 18, 2015
 *      Author: Rostislav Varzar
 */

#include "usbMSP430Defines.h"
#include "usbMSP430Interface.h"
#include "access_devices.h"

/// Init motors 1 and 2
void init_motors_12()
{
	char s1[MAX_STRING_LENGTH];	// Temp string

	makeWriteRegPacket(s1, MOTOR1, MMPER, 0x5000);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, MOTOR1, MMDUT, 0x4000);
	sendUSBPacket(s1, s1);

	makeWriteRegPacket(s1, MOTOR2, MMPER, 0x5000);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, MOTOR2, MMDUT, 0x4000);
	sendUSBPacket(s1, s1);
}

/// Start 1 and 2 motors
void start_motors_12()
{
	char s1[MAX_STRING_LENGTH];	// Temp string

	makeWriteRegPacket(s1, MOTOR1, MMCTL, MOT_ENABLE + MOT_POWER);
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

/// Init analog sensors 1, 2 and 3
void init_sensors_123()
{
	char s1[MAX_STRING_LENGTH];	// Temp string

	makeWriteRegPacket(s1, SENSOR1, SSCTL, SENS_ENABLE + SENS_READ);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, SENSOR1, SSIDX, ANALOG_INP);
	sendUSBPacket(s1, s1);

	makeWriteRegPacket(s1, SENSOR2, SSCTL, SENS_ENABLE + SENS_READ);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, SENSOR2, SSIDX, ANALOG_INP);
	sendUSBPacket(s1, s1);

	makeWriteRegPacket(s1, SENSOR3, SSCTL, SENS_ENABLE + SENS_READ);
	sendUSBPacket(s1, s1);
	makeWriteRegPacket(s1, SENSOR3, SSIDX, ANALOG_INP);
	sendUSBPacket(s1, s1);
}

/// Read data of first sensor
uint32_t read_sensor_1()
{
	char s1[MAX_STRING_LENGTH];		    // Temp string variable
	char s2[MAX_STRING_LENGTH];		    // Temp string variable
	uint32_t errcode;			    // Returned error code
	uint8_t devaddr;			    // Returned device address
	uint8_t funccode;			    // Returned function code
	uint8_t regaddr;			    // Returned register address
	uint32_t regval=UINT32_MAX;		    // Returned register value
	uint16_t tmout = 0;			    // Reading timeout

	do
	{
		makeReadRegPacket(s1, SENSOR1, SSVAL);
		errcode = sendUSBPacket(s1, s2);
		errcode = decodeReceivedPacket(s2, devaddr, funccode, regaddr, regval);
		tmout ++;
	} while (((devaddr != SENSOR1) || (regaddr != SSVAL)) && (tmout < TIME_OUT));
	return regval;
}

/// Read data of second sensor
uint32_t read_sensor_2()
{
	char s1[MAX_STRING_LENGTH];		    // Temp string variable
	char s2[MAX_STRING_LENGTH];		    // Temp string variable
	uint32_t errcode;			    // Returned error code
	uint8_t devaddr;			    // Returned device address
	uint8_t funccode;			    // Returned function code
	uint8_t regaddr;			    // Returned register address
	uint32_t regval=UINT32_MAX;		    // Returned register value
	uint16_t tmout = 0;			    // Reading timeout

	do
	{
		makeReadRegPacket(s1, SENSOR2, SSVAL);
		errcode = sendUSBPacket(s1, s2);
		errcode = decodeReceivedPacket(s2, devaddr, funccode, regaddr, regval);
		tmout ++;
	} while (((devaddr != SENSOR2) || (regaddr != SSVAL)) && (tmout < TIME_OUT));
	return regval;
}

/// Read data of third sensor
uint32_t read_sensor_3()
{
	char s1[MAX_STRING_LENGTH];		    // Temp string variable
	char s2[MAX_STRING_LENGTH];		    // Temp string variable
	uint32_t errcode;			    // Returned error code
	uint8_t devaddr;			    // Returned device address
	uint8_t funccode;			    // Returned function code
	uint8_t regaddr;			    // Returned register address
	uint32_t regval=UINT32_MAX;		    // Returned register value
	uint16_t tmout = 0;			    // Reading timeout

	do
	{
		makeReadRegPacket(s1, SENSOR3, SSVAL);
		errcode = sendUSBPacket(s1, s2);
		errcode = decodeReceivedPacket(s2, devaddr, funccode, regaddr, regval);
		tmout ++;
	} while (((devaddr != SENSOR3) || (regaddr != SSVAL)) && (tmout < TIME_OUT));
	return regval;
}
