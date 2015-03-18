/*
 * access_devices.h
 *
 *  Created on: Mar 18, 2015
 *      Author: Rostislav Varzar
 */

#ifndef ACCESS_DEVICES_H_
#define ACCESS_DEVICES_H_

/// Start 1 and 2 motors
void start_motors_12();

/// Read data of first sensor
uint32_t read_sensor_1();

/// Read data of second sensor
uint32_t read_sensor_2();

/// Read data of third sensor
uint32_t read_sensor_3();

/// Stop 1 and 2 motors
void stop_motors_12();

#endif /* ACCESS_DEVICES_H_ */
