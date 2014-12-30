/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#include "public_kernel_interface.h"
#include "assert.h"

int putchar(int);
int putchar_nobusy(int);

struct kernel_message putchar_m;
struct kernel_message * putchar_m_ptr;
struct kernel_message putchar_reply;
struct kernel_message * putchar_reply_ptr;

int putchar(int c){
	putchar_m_ptr->data = c;
	send_message(putchar_m_ptr, 6, putchar_reply_ptr);
	switch(putchar_reply_ptr->message_type){
		case MESSAGE_ACKNOWLEDGED:{
			break;
		}default:{
			assert(0, "Unknown message type.\n");
		}
	}
}

void putchar_init(void){
	/*  Set up once, and not for every character */
	putchar_m_ptr = &putchar_m;
	putchar_m_ptr->message_type = OUTPUT_CHARACTER;
	putchar_reply_ptr = &putchar_reply;
}
