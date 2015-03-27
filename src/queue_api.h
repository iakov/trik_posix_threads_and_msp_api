/*
 * queue_api.h
 *
 *  Created on: Mar 26, 2015
 *      Author: Rostislav Varzar
 */

#ifndef QUEUE_API_H_
#define QUEUE_API_H_

#include <stdint.h>
#include <pthread.h>

#define NODE_ABSENT	0
#define NODE_PRESENT	1

// Queue node
struct queue_node
{
	uint32_t thread_data;
	pthread_t thread_id;
	struct queue_node *next;
};

// Add node to queue
void push_node(uint32_t th_dat, pthread_t th_id);

// Extract node from queue
queue_node pop_node();

// Clean whole queue
void clean_queue();

// View elements of queue
void view_queue();

// Find nodes in queue by id and return number of nodes
uint32_t find_node_by_thread(pthread_t th_id);

#endif /* QUEUE_API_H_ */
