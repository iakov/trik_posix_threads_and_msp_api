//============================================================================
// Name        : trik_posix_threads_2.cpp
// Author      : Rostislav Varzar
// Version     :
// Copyright   : Free license
// Description : C++, Ansi-style
//============================================================================

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "usbMSP430Defines.h"
#include "usbMSP430Interface.h"
#include "access_devices.h"

using namespace std;

char end_flag = 0; // End flag for threads
uint32_t cnt1 = 0; // Counter for first sensor
uint32_t cnt2 = 0; // Counter for second sensor
uint32_t cnt3 = 0; // Counter for third sensor

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void reading_first_sensor()
{
	uint32_t sens_data;
	while (end_flag == 0)
	{
		pthread_mutex_lock(&mutex1);
		sens_data = read_sensor_1();
		if (sens_data < 128)
		{
			cnt1 ++;
			cout << "First sensor signal, " << cnt1 << endl;
		}
		pthread_mutex_unlock( &mutex1 );
	}
}

void reading_second_sensor()
{
	uint32_t sens_data;
	while (end_flag == 0)
	{
		pthread_mutex_lock(&mutex1);
		sens_data = read_sensor_2();
		if (sens_data < 128)
		{
			cnt2 ++;
			cout << "Second second signal, " << cnt2 << endl;
		}
		pthread_mutex_unlock( &mutex1 );
	}
}

void reading_third_sensor()
{
	uint32_t sens_data;
	while (end_flag == 0)
	{
		pthread_mutex_lock(&mutex1);
		sens_data = read_sensor_3();
		if (sens_data < 128)
		{
			cnt3 ++;
			cout << "Third third signal, " << cnt3 << endl;
		}
		pthread_mutex_unlock( &mutex1 );
	}
}


int main()
{

	pthread_t first_sensor, second_sensor, third_sensor;

	connect_USBMSP();

	init_sensors_123();

	pthread_create(&first_sensor, NULL, (void *(*)(void *))reading_first_sensor, NULL);
	pthread_create(&second_sensor, NULL, (void *(*)(void *))reading_second_sensor, NULL);
	pthread_create(&third_sensor, NULL, (void *(*)(void *))reading_third_sensor, NULL);

	cout << "Press <Enter> to stop" << endl;
	getchar();
	end_flag = 1;

	pthread_join(first_sensor, NULL);
	pthread_join(second_sensor, NULL);
	pthread_join(third_sensor, NULL);

	disconnect_USBMSP();

	pthread_exit(NULL);

	return 0;
}
