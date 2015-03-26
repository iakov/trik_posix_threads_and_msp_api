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
#include "queue_api.h"

using namespace std;

char end_flag = 0; // End flag for threads
uint32_t cnt1 = 0; // Counter for first sensor
uint32_t cnt2 = 0; // Counter for second sensor
uint32_t cnt3 = 0; // Counter for third sensor

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // Mutex to lock thread

// Thread that reads first sensor
void reading_first_sensor()
{
	uint32_t sens_data;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (end_flag == 0)
	{
		pthread_mutex_lock(&mutex1);
		sens_data = read_sensor_1();
		if (sens_data < 128)
		{
			cnt1 ++;
			cout << "First sensor signal, " << cnt1 << " Thread ID: " << pthread_self() << endl;
		}
		pthread_mutex_unlock(&mutex1);
	}
}

// Thread that reads second sensor
void reading_second_sensor()
{
	uint32_t sens_data;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (end_flag == 0)
	{
		pthread_mutex_lock(&mutex1);
		sens_data = read_sensor_2();
		if (sens_data < 128)
		{
			cnt2 ++;
			cout << "Second second signal, " << cnt2 << " Thread ID: " << pthread_self() << endl;
		}
		pthread_mutex_unlock(&mutex1);
	}
}

// Thread that reads third sensor
void reading_third_sensor()
{
	uint32_t sens_data;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (end_flag == 0)
	{
		pthread_mutex_lock(&mutex1);
		sens_data = read_sensor_3();
		if (sens_data < 128)
		{
			cnt3 ++;
			cout << "Third third signal, " << cnt3 << " Thread ID: " << pthread_self() << endl;
		}
		pthread_mutex_unlock(&mutex1);
	}
}


int main()
{

	pthread_t first_sensor, second_sensor, third_sensor;
	queue_node tmp_queue;

	// Open MSP430 USB device
	connect_USBMSP();

	init_sensors_123();

	// Creating threads
	pthread_create(&first_sensor, NULL, (void *(*)(void *))reading_first_sensor, NULL);
	pthread_create(&second_sensor, NULL, (void *(*)(void *))reading_second_sensor, NULL);
	pthread_create(&third_sensor, NULL, (void *(*)(void *))reading_third_sensor, NULL);

	cout << "ThreadID1: " << first_sensor << endl;
	cout << "ThreadID2: " << second_sensor << endl;
	cout << "ThreadID3: " << third_sensor << endl;

	// Tests for queue
	/*
	push_node(1,2);
	push_node(3,4);
	push_node(5,6);
	push_node(7,8);
	view_queue();
	clean_queue();
	view_queue();
	push_node(1,2);
	view_queue();
	clean_queue();
	view_queue();
	push_node(1,2);
	push_node(3,4);
	push_node(5,6);
	push_node(7,8);
	tmp_queue = pop_node();
	cout << tmp_queue.thread_data << endl;
	cout << tmp_queue.thread_id << endl;
	view_queue();
	tmp_queue = pop_node();
	cout << tmp_queue.thread_data << endl;
	cout << tmp_queue.thread_id << endl;
	view_queue();
	tmp_queue = pop_node();
	cout << tmp_queue.thread_data << endl;
	cout << tmp_queue.thread_id << endl;
	view_queue();
	tmp_queue = pop_node();
	cout << tmp_queue.thread_data << endl;
	cout << tmp_queue.thread_id << endl;
	view_queue();
	tmp_queue = pop_node();
	cout << tmp_queue.thread_data << endl;
	cout << tmp_queue.thread_id << endl;
	view_queue();
	 */



	cout << "Press <Enter> to stop" << endl;
	getchar();
	//end_flag = 1;

	// Terminate threads if its possible
	pthread_cancel(first_sensor);
	pthread_cancel(second_sensor);
	pthread_cancel(third_sensor);

	// Waiting while threads will end
	//pthread_join(first_sensor, NULL);
	//pthread_join(second_sensor, NULL);
	//pthread_join(third_sensor, NULL);

	// Close MSP430 USB device
	disconnect_USBMSP();

	// Exits this main thread
	pthread_exit(NULL);

	return 0;
}
