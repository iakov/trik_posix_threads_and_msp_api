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

#define MAXTIMEOUT	1024
#define ERRORTOUT	0xFFFFFFFF

char end_flag = 0; // End flag for threads
uint32_t cnt1 = 0; // Counter for first sensor
uint32_t cnt2 = 0; // Counter for second sensor
uint32_t cnt3 = 0; // Counter for third sensor

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // Mutex to lock thread

pthread_t first_sensor, second_sensor, third_sensor, test_thread;

// Send message (32 bit number) to another thread
void thread_send(uint32_t message, pthread_t threadID)
{
	push_node(message,threadID);
}

// Receive message from another thread
uint32_t thread_receive(pthread_t selfID)
{
	queue_node tmp_node;
	uint32_t timeout = 0;
	do
	{
		tmp_node = pop_node();
		if (tmp_node.thread_id != selfID)
			push_node(tmp_node.thread_data, tmp_node.thread_id);
		timeout ++;
	} while ((tmp_node.thread_id != selfID) && (timeout < MAXTIMEOUT));
	if (timeout >= MAXTIMEOUT)
		return ERRORTOUT;
	else
		return tmp_node.thread_data;
}

// Returns number of messages in queue
uint32_t thread_number_messages(pthread_t selfID)
{
	return find_node_by_thread(selfID);
}

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
		if (thread_number_messages(pthread_self()))
			cout << "First thread received message: " << thread_receive(pthread_self()) << endl;
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
		if (thread_number_messages(pthread_self()))
			cout << "Second thread received message: " << thread_receive(pthread_self()) << endl;
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
		if (thread_number_messages(pthread_self()))
			cout << "Third thread received message: " << thread_receive(pthread_self()) << endl;
		pthread_mutex_unlock(&mutex1);
	}
}

// Thread, that send test messages
void test_messages_thread()
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (end_flag == 0)
	{
		for (uint32_t d = 0; d < 10000000; d++)
			uint32_t a = d - d + d;

		pthread_mutex_lock(&mutex1);
			thread_send(111, first_sensor);
			thread_send(111, first_sensor);
			thread_send(111, first_sensor);
		pthread_mutex_unlock(&mutex1);

		for (uint32_t d = 0; d < 10000000; d++)
			uint32_t a = d - d + d;

		pthread_mutex_lock(&mutex1);
			thread_send(222, second_sensor);
			thread_send(333, third_sensor);
		pthread_mutex_unlock(&mutex1);

	}
}

int main()
{

	// Open MSP430 USB device
	connect_USBMSP();

	init_sensors_123();

	// Creating threads
	pthread_create(&first_sensor, NULL, (void *(*)(void *))reading_first_sensor, NULL);
	pthread_create(&second_sensor, NULL, (void *(*)(void *))reading_second_sensor, NULL);
	pthread_create(&third_sensor, NULL, (void *(*)(void *))reading_third_sensor, NULL);
	pthread_create(&test_thread, NULL, (void *(*)(void *))test_messages_thread, NULL);

	cout << "Press <Enter> to stop" << endl;
	getchar();
	//end_flag = 1;

	// Terminate threads if its possible
	pthread_cancel(first_sensor);
	pthread_cancel(second_sensor);
	pthread_cancel(third_sensor);
	pthread_cancel(test_thread);

	// Close MSP430 USB device
	disconnect_USBMSP();

	// Exits this main thread
	pthread_exit(NULL);

	return 0;
}
