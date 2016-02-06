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

#include <stdio.h>

int output(void);

enum foo {ENUM1, ENUM2, ENUM3};

unsigned int a(void);

unsigned int a(void){
	unsigned int i = 2;
	switch (i){
		case ENUM1:{
			i++;
		}case ENUM2:{
			if(i){
				return i;
			}
			break;
		}
	}
	return i;
}

int output(void){
        unsigned int i;
        unsigned int j;
        unsigned int k;
        unsigned int count = 17;
        char c1 = 'a';
        char c2 = 'x';
        char c3 = 'T';

	i = 0;

        for(i = 0; i < 3; i++){
		unsigned int f = 3;
		putchar('0' + (int)i + (int)f);
		if(i == 2)
			break;
		putchar('0' + (int)i);
	}

        for(i = 0; i < 3; i++){
		unsigned int f = 3;
		putchar('0' + (int)i + (int)f);
		if((int)i == 2){
			unsigned int l = 8;
			putchar('0' + (int)i + (int)l);
			break;
		}
		putchar('0' + (int)i);
	}

        for(i = 0; i < 3; i++){
		unsigned int f = 3;
		putchar('0' + (int)i + (int)f);
		if(i == 2)
			continue;
		putchar('0' + (int)i);
	}

        for(i = 0; i < 3; i++){
		unsigned int f = 3;
		putchar('0' + (int)i + (int)f);
		if(i == 2){
			unsigned int l = 8;
			putchar('0' + (int)i + (int)l);
			continue;
		}
		putchar('0' + (int)i);
	}

        for(i = 0; i < 3;){
		unsigned int f = 3;
		i++;
		putchar('0' + (int)i + (int)f);
		if(i == 2)
			break;
		putchar('0' + (int)i);
	}

        for(i = 0; i < 3;){
		unsigned int f = 3;
		i++;
		putchar('0' + (int)i + (int)f);
		if(i == 2){
			unsigned int l = 8;
			putchar('0' + (int)i + (int)l);
			break;
		}
		putchar('0' + (int)i);
	}

        for(i = 0; i < 3;){
		unsigned int f = 3;
		i++;
		putchar('0' + (int)i + (int)f);
		if(i == 2)
			continue;
		putchar('0' + (int)i);
	}

        for(i = 0; i < 3;){
		unsigned int f = 3;
		i++;
		putchar('0' + (int)i + (int)f);
		if(i == 2){
			unsigned int l = 8;
			putchar('0' + (int)i + (int)l);
			continue;
		}
		putchar('0' + (int)i);
	}
	i = 0;
	while(i < 10){
		i++;
		putchar('0' + (int)i);
		if(i == 4)
			break;
		putchar('0' + (int)i);
	}

	i = 0;
	while(i < 10){
		i++;
		putchar('0' + (int)i);
		if(i == 4){
			break;
		}
		putchar('0' + (int)i);
	}

	i = 0;
	while(i < 10){
		i++;
		putchar('0' + (int)i);
		if(i == 4)
			continue;
		putchar('0' + (int)i);
	}

	i = 0;
	while(i < 10){
		i++;
		putchar('0' + (int)i);
		if(i == 4){
			continue;
		}
		putchar('0' + (int)i);
	}

	i = 0;
	while(i < 10) i++;

        for(i = 0; i < 3; i++){
		switch (i){
			case 1:{
				unsigned int t = 4;
				putchar('0' + (int)i + (int)t);
				break;
			} case 2:{
				putchar('0' + (int)i);
				continue;
			}
		}
	}


	for(i = 0; i < 10; i++){
		putchar('0' + (int)i);
		i++;
	}

	putchar('0' + (int)a());

	i = 0;

	do{
		unsigned int t;
		if(i == 5) break;
		t = i;
		putchar('0' + (int)i + (int)t);
		i++;
	} while(i < 10);

	i = 0;

	do{
		unsigned int t;
		t = i;
		putchar('0' + (int)i + (int)t);
		i++;
	} while(i < 10);

	switch (i){
		case 4:{
			putchar('4');
		}case 10:{
			putchar('z');
			putchar('z');
			putchar('z');
			putchar('z');
			putchar('z');
			putchar('z');
		}default:{
			putchar('x');
		}
	}
	i = 0;
	putchar('0' + (int)i);
	switch (i){
		case 4:{
			putchar('4');
		}case 10:{
			putchar('z');
			putchar('1');
			putchar('2');
			putchar('3');
			putchar('z');
			putchar('z');
		}default:{
			putchar('x');
		}
	}

	for(i = 0; i < 10; i++){
		unsigned int dd = 4;
		unsigned int foo = 4, boo;
		boo = 5;
		putchar('0' + (int)i + (int)dd);
		{
			unsigned int tt = 9, l = 8;
			unsigned int e;
			unsigned int f;
			e = 3;
			f = 11;
			putchar('i');
			putchar('0' + (int)i);
			putchar('j');
			putchar('0' + (int)dd);
			putchar('k');
			putchar('0' + (int)tt + (int)l);
			putchar('0' + (int)e);
			putchar('0' + (int)f);
		}


		{
			unsigned int l = 9, tt = 8;
			unsigned int f;
			unsigned int e;
			e = 2;
			f = 6;
			putchar('i');
			putchar('0' + (int)i);
			putchar('j');
			putchar('0' + (int)f);
			putchar('k');
			putchar('0' + (int)tt + (int)l);
			putchar('0' + (int)e);
			putchar('0' + (int)f);
			putchar('0' + (int)foo);
			putchar('0' + (int)boo);
		}
		i++;
	}

	for(i = 0; i < 10;){
		putchar('0' + (int)i);
		i++;
	}
	i = 0;
	while(i < 4){
		unsigned int a = 0;
		{ {{{{{{{{{{{{{{{{{{{ unsigned int b = 9; putchar('0' + (int)a + (int)b); }}}}}}}}{{{{{unsigned int b = 9; putchar('0' + (int)a + (int)b);}}}}}{{{{unsigned int b = 9; putchar('0' + (int)a + (int)b);}}}}}}}}}}}}}}}}
		i++;
	}

	if(3 == 3){
		unsigned int tt = 0;
		unsigned int ff = 3;
		putchar('0' + (int)tt);
		putchar('0' + (int)ff);
	}


	i = 0;
	while(i < 10){
		i++;
		if(i == 4){
			continue;
		}
		putchar('0' + (int)i);
	}

	i = 0;
	while(i < 10){
		i++;
		if(i == 4){
			break;
		}
		putchar('0' + (int)i);
	}


	(void)j;
        for(k = 0; k < 3; k++){
		putchar('0' + (int)k);
		if(k)
			continue;
		putchar('0' + (int)k);
	}

        for(k = 0; k < 3;){
		k++;
		putchar('0' + (int)k);
		if(k)
			continue;
		putchar('0' + (int)k);
	}
        for(k = 0; k < 3; k++){
                for(i = 0; i < 3; i++)
                        for(j = 0; j < 3; j++)
                                if(1)
                                        switch (j)
                                                case 0:
							switch (j)
								case 0:
									continue;
		putchar('0' + (int)k);
        }

        for(k = 0; k < 3; k++){
                unsigned int t = 9;
                for(i = 0; i < 3; i++)
                        for(j = 0; j < 3; j++)
                                if(1)
                                        switch (j)
                                                case 0:
							switch (j)
								case 0:
									putchar('0' + (int)k + (int)t);
		putchar('0' + (int)k);
        }

	i = 1;

	switch (i){
		case 1:{
			putchar('l');
			putchar('0' + (int)i);
			break;
		} case 2:{
			putchar('o');
			putchar('0' + (int)i);
		}
	}


	for(i = 0; i < 3; i++){
		unsigned int t = 4;
		switch (i){
			case 1:{
				unsigned int g = 1;
				putchar('0' + (int)i + (int)t + (int)g);
				break;
			} case 2:{
				unsigned int g = 1;
				putchar('0' + (int)i + (int)g);
				continue;
			}
		}
		putchar('0' + (int)i);
	}
        
	for(k = 0; k < 3; k++)
                for(i = 0; i < 3; i++){
                        switch (i){
                                case 1:{
					unsigned int t = 4;
label1:
					putchar('0' + (int)i + (int)t);
					if(t == 99999){
						goto label1;
					}
                                        break;
                                } case 2:{
					putchar('0' + (int)i);
                                        continue;
                                }
                        }
			putchar('0' + (int)i);
                }
	putchar('x');

	i = 8;
	putchar('0' + (int)--i);

	do{
		putchar('0' + (int)i);
	}while (--i > 0);

	(void)count;
	putchar('S');

	i = (count + 7) / 8;
	putchar('0' + (int)count);
	putchar('0' + (int)(count + 7));
	putchar('0' + (int)i);
	putchar('0' + (int)(count % 8));
	switch(count % 8) {
	case 0:	do {	putchar('0' + (int)i);
	case 7:		putchar('0' + (int)i);
	case 6:		putchar('0' + (int)i);
	case 5:		putchar('0' + (int)i);
	case 4:		putchar('0' + (int)i);
	case 3:		putchar('0' + (int)i);
	case 2:		putchar('0' + (int)i);
	case 1:		putchar('0' + (int)i);
		} while(--i > 0);
	}


	switch(c1){
		case 'a':{
			putchar('a');
			break;
		}default:{
			putchar('d');
		}
	}

	switch(c2){
		case 'a':{
			putchar('a');
			break;
		}case 'T':{
			putchar('T');
			break;
		}default:{
			putchar('d');
		}
	}

	switch(c3){
		case 'a':{
			putchar('a');
			break;
		}case 'T':{
			putchar('T');
			break;
		}default:{
			putchar('d');
		}
	}

	return 0;
}

int main(void){
	output();
	return 0;
}
