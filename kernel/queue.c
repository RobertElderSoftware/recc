/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#include "queue.h"
#include <assert.h>

void task_queue_init(struct task_queue * queue, unsigned int size) {
	queue->start = 0;
	queue->end = 0;
	queue->current_count = 0;
	queue->size = size;
}

void task_queue_push_end(struct task_queue * queue, void * item) {
	if ((queue->end + 1) % queue->size == queue->start) {
		assert(0 && "Task queue is full.\n");
	}
	
	queue->items[queue->end] = item;
	queue->end = (queue->end + 1) % queue->size;
	queue->current_count += 1;
}

void * task_queue_pop_start(struct task_queue * queue) {
	void * item;
	if (queue->start == queue->end) {
		assert(0 && "Task queue is empty.\n");
		return (void *)0;
	}

	item = queue->items[queue->start];

	queue->start = (queue->start + 1) % queue->size;
	queue->current_count -= 1;
	return item;
}

unsigned int task_queue_current_count(struct task_queue * queue){
	return queue->current_count;
}

void message_queue_init(struct message_queue * queue, unsigned int size){
	queue->start = 0;
	queue->end = 0;
	queue->current_count = 0;
	queue->size = size;
}

void message_queue_push_end(struct message_queue * queue, struct kernel_message item){
	if ((queue->end + 1) % queue->size == queue->start) {
		assert(0 && "Message queue is full.\n");
	}
	
	queue->items[queue->end] = item;
	queue->end = (queue->end + 1) % queue->size;
	queue->current_count += 1;
}

struct kernel_message message_queue_pop_start(struct message_queue * queue){
	struct kernel_message item;
	if (queue->start == queue->end) {
		assert(0 && "Message queue is empty.\n");
		return item;
	}

	item = queue->items[queue->start];

	queue->start = (queue->start + 1) % queue->size;
	queue->current_count -= 1;
	return item;
}


unsigned int message_queue_current_count(struct message_queue * queue){
	return queue->current_count;
}
