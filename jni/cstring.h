
/*
 * Copyright (C) 2018 by Alexzander Purwoko Widiantoro
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
 *	@(#)cstring.h	v0.5 @@ HEADER FILES
 * Last updated in : Sunday March, 18, 2018 on 11:57AM
 * Time Zone : Jakarta, GMT+7 , WIB
 */


#ifndef _CSTRING_H_
#define _CSTRING_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS
typedef struct __c_str_inode_ {
	char __c;
	struct __c_str_inode_ *__next;
}__str_inode_;
typedef struct __str_element_ {
	__str_inode_ *__s_temp;
	__str_inode_ *__s_current;
	size_t _length;
	size_t __current_str_pos;
}string;
extern string *snew();
extern void scat(string *to, string *from);
extern void sappend(string *to, const char *fmt, ...);
extern void sadd(string *s, const char c);
extern int scmp(string *s, string *t);
extern size_t slen(string *s);
extern void sshow(string *s);
extern void sfree(string *s);
extern void incps(string *s);
extern void decps(string *s);
extern size_t sgcpp(string *s);
extern void sresp(string *s);
extern char sgetc(string *s);
extern char sgetcp(string *s, size_t pos);
extern void sget(string *s, char *to, size_t start, size_t end);
extern void ssetc(string *s, char c);
extern void ssetcp(string *s, char c, size_t pos);
extern void sset(string *to, const char *s, size_t start, size_t end);
extern void sdel(string *s, size_t pos);
extern void sdelp(string *s, size_t start, size_t end);
extern void sdelc(string *s, char c);
//extern void sdelco(string *s, char c);
extern int smatch(string *s, const char *source);
extern void sreverse(string *s);
extern void stoupper(string *s);
extern void stolower(string *s);
// for conversion
extern int stoi(string *s);
extern double stof(string *s);
extern long stol(string *s);
extern long long stoll(string *s);
extern string *itos(int data);
extern string *ltos(long data);
extern string *lltos(long long data);
extern string *ftos(double data);

extern string *itohex(unsigned int data);
extern string *itob(unsigned int data);
extern string *itooct(unsigned int data);
__END_DECLS

#endif // _CSTRING_H_

