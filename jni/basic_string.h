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
 *	@(#)basic_string.h	v1.2 @@ HEADER FILE @@
 * Last updated in : Tuesday Nov, 23, 2017 in 08:29 PM
 * Time Zone : Jakarta, GMT+7 , WIB
 */
#include "component.h"
#define B_MAXSIZE 16384
#define B_MAXBUFF 8192
typedef struct __string__basic {
	char __content[B_MAXSIZE];
	char __B_BUFF[B_MAXBUFF];
	int __lastLen;
	int __lastPos;
	short __lock;
	short __errnum;
}bstring;
bstring* bnew();
void bappend(bstring* str, const char *format, ...);
void bconcat(bstring* str, const bstring* t);
void bset(bstring* str, const char* source, int start_position, int length_source);
char *sub_bstr(bstring* str, int start_position, int end_position);
void bdel_pos(bstring* str, int start_index, int end_index);
size_t blen(bstring* str);
int berrors(bstring* str);
char *bget(bstring* str);
void brelease(bstring* str);

void bresetp(bstring* str);
char breadp(bstring* str);
int b_isnp(bstring* str);
void bsetp(bstring* str, char s);
int bmatchp(bstring* str, char *pattern);
void bdelp(bstring* str);

