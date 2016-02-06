/*
    Copyright 2016 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
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
