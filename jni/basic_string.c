/*
 * Copyright (C) 2017 Alexzander Purwoko Widiantoro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)basic_string.c	v1.2 @@ SOURCE FILE @@
 * Last updated in : Tuesday Nov, 23, 2017 in 08:29 PM
 * Time Zone : Jakarta, GMT+7 , WIB
 */
#include "basic_string.h"
bstring* bnew(){
	bstring* b = malloc(sizeof(bstring));
	b->__lock = -1;
	b->__errnum = b->__lastPos = b->__lastLen = 0;
	return b;
}
void bappend(bstring* str, const char *format, ...){
	va_list args;
	va_start(args, format);
	vsprintf(str->__B_BUFF, format, args);
	int clen = strlen(str->__B_BUFF);
	if(str->__lastLen == B_MAXSIZE || ((str->__lastLen)+clen) >= B_MAXSIZE)
	{
		str->__errnum = -1;
		va_end(args);
		return;
	}
	register int c,  count = 0;
	for(; (c = str->__B_BUFF[count]) != '\0';count++)
		str->__content[str->__lastLen++] = c;
	str->__content[str->__lastLen] = '\0';
	str->__errnum = 0;
	va_end(args);
	return;
}
void bconcat(bstring* str, const bstring* t){
	bappend(str,bget(t));
}
void bset(bstring* str, const char* source, int start_position, int length_source){
	if(start_position >= str->__lastLen || (start_position+length_source)>= str->__lastLen ){
		fprintf(stderr, "Error code -1 : because start_position is greater or equal than length of str or your string has reached the B_MAXSIZE");
		str->__errnum = -1;
		return;
	}
	register int c;
	int x = 0;
	for(; x < length_source; x++)
		str->__content[start_position++] = source[x];
	str->__errnum = 0;
	return;
}
char *sub_bstr(bstring* str, int start_position, int end_position){
	if(start_position < 0 || end_position >= str->__lastLen){
		fprintf(stderr, "Error code -1: start_position < 0 or end_position is reaching end of length str");
		str->__errnum = -1;
		return NULL;
	}
	else if((end_position - start_position) >= B_MAXBUFF){
		fprintf(stderr, "Error code -2: start_position - end_position >= B_MAXBUFF");
		str->__errnum = -2;
		return NULL;
	}
	int pos = 0;
	for(; start_position <= end_position; start_position++)
		str->__B_BUFF[pos++] = str->__content[start_position];
	str->__B_BUFF[pos] = '\0';
	str->__errnum = 0;
	return str->__B_BUFF;
}
void bdel_pos(bstring* str, int start_index, int end_index){
	if(start_index < 0 || start_index > end_index){
		fprintf(stderr, "Error code -1: start_position < 0 or start_position > end_index");
		str->__errnum = -1;
		return;
	}
	else if(end_index >= str->__lastLen){
		fprintf(stderr, "Error code -2:  end_index is reaching end of length str");
		str->__errnum = -2;
		return;
	}
	int pos1 = end_index + 1;
	int mstart = start_index;
	register char c;
	while((c = str->__content[pos1++])!='\0')
		str->__content[start_index++] = c;
	str->__content[start_index] = '\0';
	printf("\n%d\n", (str->__lastLen) );
	str->__lastLen = (str->__lastLen) - ((end_index-mstart)+ 1);
	str->__errnum = 0;
	return;
}
int b_isnp(bstring* str){
	return (str->__lock == 1 && str->__lastPos >= blen(str)) ? 1 : 0;
}
char breadp(bstring* str){
	if(str->__lastPos >= blen(str) && str->__lock == 1){
		str->__errnum = 1;
		fprintf(stderr, "Error code 1: index lastPos is reaching end of character buffer size and lock Pointer == 1 ");
		return '\0';//NULL
	}
	if(str->__lock == -1)str->__lock = 0;
	if(str->__lock == 0){
		// Get Char
		char c = str->__content[str->__lastPos];
		// increment lastPos
		str->__lastPos++;
		// if lastPos >= MAXSIZE THEN lock = 1
		// indicates failure to reading
		if(str->__lastPos >= blen(str))
			str->__lock = 1;
		return c;
	}
	return '\0';
}
void bresetp(bstring* str){
	str->__lastPos = 0;
	str->__lock = -1;
	return;
}
void bsetp(bstring* str, char s){
	if(str->__lastPos >= blen(str) && str->__lock == 1){
		str->__errnum = 1;
		fprintf(stderr, "Error code 1: index lastPos is reaching end of character buffer size and lock Pointer == 1 ");
		return ;
	}
	if(str->__lock == -1)str->__lock = 0;
	if(str->__lock == 0 && str->__lastPos < blen(str))
		str->__content[str->__lastPos] = s;
	return;
}
int bmatchp(bstring* str, char *pattern){
	int lenpattern 		= strlen(pattern),
		lenstr 			= blen(str),
		count 			= 0,
		point_str 		= 0,
		count_pattern 	= 0,
		point_pattern 	= 0;
	for(; point_str < lenstr; point_str++){
		
		if(str->__content[point_str] == pattern[point_pattern]){
			count++;
			point_pattern++;
			point_str++;
			while(point_pattern < lenpattern){
				if(str->__content[point_str] == pattern[point_pattern])
					count++;
				point_pattern++;
				point_str++;
			}
			if(count == lenpattern)count_pattern++;
			point_pattern = count = 0;
			point_str--;
		}
	}
	return count_pattern;
}
void bdelp(bstring* str){
	if(str->__lastPos >= blen(str) && str->__lock == 1){
		str->__errnum = 1;
		fprintf(stderr, "Error code 1: index lastPos is reaching end of character buffer size and lock Pointer == 1 ");
		return ;
	}
	if(str->__lock == -1)str->__lock = 0;
	if(str->__lock == 0 && str->__lastPos < blen(str))
		bdel_pos(str, str->__lastPos, str->__lastPos);
	return;
}
size_t blen(bstring* str){
	return str->__lastLen;
}
int berrors(bstring* str){
	return str->__errnum;
}
char* bget(bstring* str){
	return str->__content;
}
void brelease(bstring* str){
	free(str);
}
