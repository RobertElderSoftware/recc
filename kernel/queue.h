#ifndef __QUEUE_H_DEFINED__
#define __QUEUE_H_DEFINED__
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

#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif
#ifndef __CORE_DATA_TYPES_H
#include "core_data_types.h"
#endif

struct task_queue {
	unsigned int start;
	unsigned int end;
	unsigned int current_count;
	unsigned int size;
	void * items[MAX_NUM_PROCESSES];
};

struct message_queue {
	unsigned int start;
	unsigned int end;
	unsigned int current_count;
	unsigned int size;
	struct kernel_message items[MAX_NUM_PROCESSES];
};

void task_queue_init(struct task_queue *, unsigned int);

void task_queue_push_end(struct task_queue *, void *);

void * task_queue_pop_start(struct task_queue *); 

unsigned int task_queue_current_count(struct task_queue *); 

void message_queue_init(struct message_queue *, unsigned int);

void message_queue_push_end(struct message_queue *, struct kernel_message);

struct kernel_message message_queue_pop_start(struct message_queue *); 

unsigned int message_queue_current_count(struct message_queue *); 

#endif
