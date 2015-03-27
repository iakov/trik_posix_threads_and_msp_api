/*
 * queue_api.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: Rostislav Varzar
 */

#include <iostream>

#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#include "queue_api.h"

using namespace std;

queue_node *QuFirst = NULL, *QuLast = NULL; // First and Last pointer to queue

// Add node to queue
void push_node(uint32_t th_dat, pthread_t th_id)
{
	queue_node *qqq = new queue_node;
	qqq->thread_data = th_dat;
	qqq->thread_id = th_id;
	qqq->next = NULL;
	if (QuFirst == NULL)
		QuLast = QuFirst = qqq;
	else
	{
		QuLast->next = qqq;
		QuLast = qqq;
	}
}

// Extract node from queue
queue_node pop_node()
{
	queue_node aaa;
	if (QuFirst == NULL)
	{
		aaa.thread_data = 0;
		aaa.thread_id = 0;
		return aaa;
	}
	queue_node *qqq = QuFirst;
	aaa.thread_data = qqq->thread_data;
	aaa.thread_id = qqq->thread_id;
	QuFirst = qqq->next;
	if (QuFirst == NULL)
		QuLast = NULL;
	delete qqq;
	return aaa;
}

// Find nodes in queue by id and return number of nodes
uint32_t find_node_by_thread(pthread_t th_id)
{
	uint32_t retpos = NODE_ABSENT;
	for (queue_node *qqq = QuFirst; qqq != NULL; qqq = qqq->next)
	{
		if (qqq->thread_id == th_id)
			retpos ++;
	}
	return retpos;
}

// Clean whole queue
void clean_queue()
{
	for (queue_node *qqq = QuFirst; qqq != NULL; qqq = qqq->next)
		delete qqq;
	QuFirst = QuLast = NULL;
}

// View elements of queue
void view_queue()
{
	for (queue_node *qqq = QuFirst; qqq != NULL; qqq = qqq->next)
		cout << qqq->thread_data << " " << qqq->thread_id << endl;
}
