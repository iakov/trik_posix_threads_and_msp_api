/*
 * usbMSP430Interface.cpp
 *
 *  Created on: March 17, 2015
 *	Author: Rostislav Varzar
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <ctype.h>
#include <iostream>

#include "usbMSP430Defines.h"
#include "usbMSP430Interface.h"

volatile uint16_t mper; // Global PWM motor period
int usb_out_descr; // Input/Output USB device descriptor
struct termios usb_tty; // Struct for termio parameters, MUST BE GLOBAL!!!

/// Extract number from packet
uint32_t hex2num(char *string, uint16_t pos, uint16_t numsize)
{
	uint32_t resnum = 0;
	uint32_t tmpnum = 0;
	char c = 0;
	for (uint16_t i = 0; i < numsize; i++)
	{
		c = toupper(string[pos + i]);
		tmpnum = c > '9' ? c - 'A' + 10 : c - '0';
		resnum |= (tmpnum << ((numsize - i - 1) * 4));
	}
	return resnum;
}

/// Make write register packet
void makeWriteRegPacket(char *msp_packet, uint8_t dev_addr, uint8_t reg_addr,
		uint32_t reg_val)
{
	uint8_t crc = (0xFF - (dev_addr + WRITE_FUNC + reg_addr + uint8_t(
			reg_val & 0xFF) + uint8_t((reg_val >> 8) & 0xFF)
			+ uint8_t((reg_val >> 16) & 0xFF) + uint8_t(
			(reg_val >> 24) & 0xFF)) + 1) & 0xFF; // Checksum
	sprintf(msp_packet, ":%02X%02X%02X%08X%02X\n", dev_addr, WRITE_FUNC,
			reg_addr, reg_val, crc);
}

/// Make read register packet
void makeReadRegPacket(char *msp_packet, uint8_t dev_addr, uint8_t reg_addr)
{
	uint8_t crc = (0xFF - (dev_addr + READ_FUNC + reg_addr) + 1) & 0xFF; // Checksum
	sprintf(msp_packet, ":%02X%02X%02X%02X\n", dev_addr, READ_FUNC,
			reg_addr, crc);
}

/// Init USB TTY device
uint32_t init_USBTTYDevice()
{
	memset(&usb_tty, 0, sizeof(usb_tty));

	if (tcgetattr(usb_out_descr, &usb_tty) != 0)
	{
		//cout << "Error " << errno << " from tcgetattr: " << strerror(errno);
		return DEVICE_ERROR;
	}

	cfsetospeed(&usb_tty, __MAX_BAUD);
	cfsetispeed(&usb_tty, __MAX_BAUD);

	usb_tty.c_cflag &= ~PARENB; // Make 8n1
	usb_tty.c_cflag &= ~CSTOPB;
	usb_tty.c_cflag &= ~CSIZE;
	usb_tty.c_cflag |= CS8;
	usb_tty.c_cflag &= ~CRTSCTS; // No flow control
	usb_tty.c_lflag = 0; // No signaling chars, no echo, no canonical processing
	usb_tty.c_oflag = 0; // No remapping, no delays
	usb_tty.c_cc[VMIN] = 0; // Read doesn't block
	usb_tty.c_cc[VTIME] = 1; // 0.1 seconds read timeout
	usb_tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines
	usb_tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	usb_tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Make raw
	usb_tty.c_oflag &= ~OPOST; // Make raw

	tcflush(usb_out_descr, TCIFLUSH);

	if (tcsetattr(usb_out_descr, TCSANOW, &usb_tty) != 0)
	{
		// qDebug() << "Error " << errno << " from tcsetattr";
		return DEVICE_ERROR;
	}

	return NO_ERROR;
}

/// Send USB packet
uint32_t sendUSBPacket(char *in_msp_packet, char *out_msp_packet)
{
	uint32_t tout = 0; // Timeout counter
	int32_t n_written = 0; // Number of written bytes
	int32_t n_read = 0; // Number of read bytes
	size_t n_write = 0; // Number to write bytes
	char s1[MAX_STRING_LENGTH]; // Temp string

	if (usb_out_descr < 0)
	{
		// qDebug() << "Error device descriptor" << errno << " : " << strerror (errno);
		return DEVICE_ERROR;
	}

	// Write packet
	n_write = strlen(in_msp_packet);
	sprintf(s1, in_msp_packet);
	n_written = write(usb_out_descr, s1, n_write);
	if (n_written != strlen(s1))
	{
		// qDebug() << "Error writing: " << strerror(errno);
		return PACKET_ERROR;
	}
	tcflush(usb_out_descr, TCOFLUSH);

	// Read packet
	memset(&s1, '\0', MAX_STRING_LENGTH);
	do
	{
		n_read = read(usb_out_descr, &s1, MAX_STRING_LENGTH);
		tout++;
	} while ((n_read < RECV_PACK_LEN) && (tout < TIME_OUT));
	tcflush(usb_out_descr, TCIFLUSH);

	if ((n_read != RECV_PACK_LEN) || (tout == TIME_OUT))
	{
		// qDebug() << "Error reading: " << strerror(errno);
		sprintf(out_msp_packet, "\0");
		return PACKET_ERROR;
	}
	else
	{
		s1[18] = 0x00;
		strncpy(out_msp_packet, s1, n_read + 1);
	}

	return NO_ERROR;
}

/// Connect to USB MSP430 device
uint32_t connect_USBMSP()
{
	// Open USB descriptor for writing
	usb_out_descr = open(USB_DEV_NAME, O_RDWR | O_NONBLOCK | O_NDELAY);
	if (usb_out_descr < 0)
	{
		// qDebug() << "Error " << errno << " opening " << USB_DEV_NAME << ": " << strerror (errno);
		return DEVICE_ERROR;
	}

	// Init USB STTY device with serial port parameters
	init_USBTTYDevice();

	return NO_ERROR;
}

/// Disconnect from USB MSP430 device
uint32_t disconnect_USBMSP()
{
	if (usb_out_descr < 0)
	{
		// qDebug() << "Error device descriptor" << errno << " : " << strerror (errno);
		return DEVICE_ERROR;
	}
	close(usb_out_descr);

	return NO_ERROR;
}

