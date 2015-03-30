/*
 * mess_module.cpp
 *
 *  Created on: Mar 30, 2015
 *      Author: Rostislav Varzar
 */

#include "mess_module.h"
#include "queue_api.h"

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
uint32_t thread_get_number_messages(pthread_t selfID)
{
	return find_node_by_thread(selfID);
}
