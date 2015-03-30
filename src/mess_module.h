/*
 * mess_module.h
 *
 *  Created on: Mar 30, 2015
 *      Author: Rostislav Varzar
 */

#ifndef MESS_MODULE_H_
#define MESS_MODULE_H_

#include <pthread.h>
#include <stdint.h>

#define MAXTIMEOUT	1024
#define ERRORTOUT	0xFFFFFFFF

// Send message (32 bit number) to another thread
void thread_send(uint32_t message, pthread_t threadID);

// Receive message from another thread
uint32_t thread_receive(pthread_t selfID);

// Returns number of messages in queue
uint32_t thread_get_number_messages(pthread_t selfID);

#endif /* MESS_MODULE_H_ */
