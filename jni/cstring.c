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
 *	@(#)cstring.c	v0.5 @@ SOURCE FILES @@ LIMITED ONLY
 * Last updated in : Sunday March, 18, 2018 on 11:57 AM
 * Time Zone : Jakarta, GMT+7 , WIB
 */

#include "cstring.h"
#ifdef _CSTRING_H_
///////  INTERFACE FUNCTION \\\\\\\\
/**** DO NOT MODIFY THIS SECTION *****/

/** basic for allocating memory and getchar **/
__str_inode_ *__create_strNodes(){
	return (__str_inode_ *) malloc(sizeof(__str_inode_));
}

__str_inode_ *__insert_strNodes(__str_inode_ *__curr, __str_inode_ *__temp){
	if(!__curr)return __temp;
	__curr->__next = __insert_strNodes(__curr -> __next, __temp);
}

__str_inode_ *__release_strNodes(__str_inode_ *__curr){
	__str_inode_ *__tmp;
	while(__curr){
		__tmp = __curr;
		__curr = __curr -> __next;
		free(__tmp);
		__tmp = NULL;
	}
	return __curr;
}

__str_inode_ *__delete_c_in_strNodes(__str_inode_ *__curr, char __c){
	__str_inode_ *__tmp;
	while(__curr){
		if(__curr -> __c == __c){
			__tmp = __curr;
			__curr = __curr -> __next;
			free(__tmp);
			__tmp = NULL;
			return __curr;
		}
	}
	return __curr;
}
__str_inode_ *__reverse_strNodes(__str_inode_ *__curr){
	__str_inode_ *__nxt, *__temp;
	if(!__curr) return(__curr);
	else
	{
		__nxt = __curr -> __next;
		__curr -> __next = NULL;
		while(__nxt)
		{
			__temp = __nxt->__next;
			__nxt->__next = __curr;
			__curr = __nxt;
			__nxt = __temp;
		}
		return __curr; 
	}
}

int __isFound_c_strNodes(__str_inode_ *__curr, char __c){
	while(__curr)
		if(__curr -> __c == __c)
			return 1;
		else
			__curr = __curr -> __next;
	return 0;
}

size_t __findPos_strNodes(__str_inode_ *__curr, char __c){
	size_t __pos = 0;
	while(__curr)
		if(__curr -> __c == __c)
			return __pos;
		else {
			__pos++;
			__curr = __curr -> __next;
		}
	return -1;
}

void __setPos_strNodes(__str_inode_ *__curr, size_t __pos, char __c){
	size_t __cp = 0;
	while(__curr)
		if(__cp == __pos)
			__curr -> __c = __c;
		else {
			__cp++;
			__curr = __curr -> __next;
		}
}
// getting in original mode
char __get_c_in_strNodes(__str_inode_ *__curr, size_t __pos){
	size_t __cp = 0;
	while(__curr)
		if(__cp == __pos)
			return (__curr -> __c);
		else {
			__cp++;
			__curr = __curr -> __next;
		}
	return 0;
}
// getting in recursive mode
char __get_c_in_strNodes__rec(string *__curr){
	size_t *__pos = &(__curr -> __current_str_pos);
	size_t __cp = 0;
	while(__curr -> __s_current)
		if(__cp == (*__pos)){
			*__pos = (*__pos) + 1;
			return __curr -> __s_current -> __c;
		}
		else {
			__cp++;
			__curr -> __s_current = __curr -> __s_current ->__next;
		}
	return 0;
}
void __reset_strNodes(string *__str){
	__str -> __current_str_pos = 0;
}


/////////// VISIBLE FUNCTION \\\\\\\\\\\\\\\

string *snew(){
	string *__sn =  (string *)malloc(sizeof(string));
	__sn -> __s_current = NULL;
	__sn -> _length = 0;
	__sn -> __current_str_pos = 0;
	return __sn;
}
void scat(string *to, string *from){
	while(from -> __s_current){
		to -> __s_temp = __create_strNodes();
		to -> __s_temp -> __c = from -> __s_current -> __c;
		to -> __s_temp -> __next = NULL;
		to -> __s_current = __insert_strNodes(to -> __s_current, to -> __s_temp);
		from -> __s_current = from -> __s_current -> __next;
	}
	to -> _length = (to -> _length) + (from -> _length);
}
void sappend(string *to, const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	char buffer[1024];
	vsprintf(buffer, fmt, args);
	register int x = 0;
	while(buffer[x] != '\0'){
		to -> __s_temp = __create_strNodes();
		to -> __s_temp -> __c = buffer[x++];
		to -> __s_temp -> __next = NULL;
		to -> __s_current = __insert_strNodes(to -> __s_current, to -> __s_temp);
		
	}
	to -> _length = (to -> _length) + strlen(buffer);
	va_end(args);
}
void sadd(string *to, const char c){
	if(!to)return;
	to -> __s_temp = __create_strNodes();
	to -> __s_temp -> __c = c;
	to -> __s_temp -> __next = NULL;
	to -> __s_current = __insert_strNodes(to -> __s_current, to -> __s_temp);
	to -> _length = (to -> _length) + 1;
}
int scmp(string *s, string *t){
	int i = 0;
	__str_inode_ *_s = s -> __s_current;
	__str_inode_ *_t = t -> __s_current;
	int (*_gt) (__str_inode_ *, char) = 
		(int (*) (__str_inode_ *, char))__get_c_in_strNodes;
	for(; ((*_gt)(_s, i)) == ((*_gt)(_t, i)); i++)
		if(((*_gt)(_s, i)) == 0) return 0;
	return (((*_gt)(_s, i)) - ((*_gt)(_t, i)));
}
size_t slen(string *str){
	return str -> _length;
}
void sshow(string *str){
	__str_inode_ *tmp = str -> __s_current;
	while(tmp){
		printf("%c", tmp -> __c);
		tmp = tmp -> __next;
	}
	printf("\n");
}
void sfree(string *str){
	str -> __s_current = __release_strNodes(str -> __s_current);
	free(str);
}
void decps(string *__str){
	if(__str -> __current_str_pos != 0)
		__str -> __current_str_pos = __str -> __current_str_pos -1;
}
void incps(string *__str){
	if(__str -> __current_str_pos < __str -> _length)
	__str -> __current_str_pos = __str -> __current_str_pos + 1;
}
char sgetc(string *s){
	return __get_c_in_strNodes__rec(s);
}
char sgetcp(string *s, size_t pos){
	return __get_c_in_strNodes(s -> __s_current, pos);
}
void sget(string *s, char *to, size_t start, size_t end){
	__str_inode_ *__tmp = s -> __s_current;
	if(end >= s -> _length)return;
	register size_t __tp = 0;
	for(; start<=end; start++){
		char c = __get_c_in_strNodes(__tmp, start);
		to[__tp++] = c;
	}
	to[__tp] = '\0';
}
void ssetc(string *s, char c){
	__setPos_strNodes(s -> __s_current, s -> __current_str_pos, c);
}
void ssetcp(string *s, char c, size_t pos){
	__setPos_strNodes(s -> __s_current, pos, c);
}
void sset(string *to, const char *s, size_t start, size_t end){
	size_t __lenfmt = strlen(s);
	if(end >= to -> _length)return;
	register size_t x = 0;
	for(; start <= end; start++, x++)
		if(x < __lenfmt)
			ssetcp(to, s[x], start);
}
void sdel(string *s, size_t pos){
	__str_inode_ *__tmp, *__curr = s -> __s_current;
	register size_t __pos = 0;
	while(__curr){
		if(__pos == pos){
			__tmp = __curr;
			__curr = __curr -> __next;
			free(__tmp);
			__tmp = NULL;
			s -> _length = s -> _length - 1;
			break;
		}
		__pos++;
	}
	s -> __s_current = __curr;
}
void sdelc(string *s, char __c){
	__str_inode_ *__tmp, *__curr = s -> __s_current;
	while(__curr){
		if(__curr -> __c == __c){
			__tmp = __curr;
			__curr = __curr -> __next;
			free(__tmp);
			__tmp = NULL;
			s -> _length = s -> _length - 1;
			break;
		}
	}
	s -> __s_current = __curr;
}
/*void sdelco(string *s, char __c){
	__str_inode_ *a = s -> __s_current,
				 *b,
				 *c = s -> __s_current,
				 *d;
	register short x = 0;
	while(c){
		if(c -> __c == __c){
			b = c;
			c = c -> __next;
			free(b);
		}
		else {
			if(x == 0){
				a -> __c = c -> __c;
				d = a;
				a = a -> __next;
				x++;
			}
			else {
				a -> __c = c -> __c;
				a = a -> __next;
			}
			c = c -> __next;
		}
	}
	a -> __next = NULL,
	a -> __c = 0;
	s -> __s_current = d;
}*/
void sdelp(string *s, size_t start, size_t end){
	if(end >= s -> _length)return;
	__str_inode_ *__tmp, *__curr = s -> __s_current;
	while(__curr){
		if(start <= end){
			__tmp = __curr;
			__curr = __curr -> __next;
			free(__tmp);
			__tmp = NULL;
			s -> _length = s -> _length - 1;
		}
		else break;
		start++;
	}
	s -> __s_current = __curr;
}
size_t sgcpp(string *s){
	return s -> __current_str_pos;
}
void sresp(string *s){
	s -> __current_str_pos = 0;
}
int smatch(string *s, const char *source){
	int lenpattern 		= strlen(source),
		lenstr 			= s -> _length,
		count 			= 0,
		point_str 		= 0,
		count_pattern 	= 0,
		point_pattern 	= 0;
	for(; point_str < lenstr; point_str++){
		
		if(sgetcp(s, point_str) == source[point_pattern]){
			count++;
			point_pattern++;
			point_str++;
			while(point_pattern < lenpattern){
				if(sgetcp(s, point_str) == source[point_pattern])
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
void sreverse(string *s){
	__str_inode_ *__tmp = s -> __s_current;
	s -> __s_current = __reverse_strNodes(__tmp);
}
void stoupper(string *s){
	__str_inode_ *__curr = s -> __s_current;
	register char __c;
	while(__curr){
		__c = __curr -> __c;
		__curr -> __c = ((__c >= 'a' && __c <= 'z')? ('a' - 'A') - __c : __c);
		__curr = __curr -> __next;
	}
}
void stolower(string *s){
	__str_inode_ *__curr = s -> __s_current;
	register char __c;
	while(__curr){
		__c = __curr -> __c;
		__curr -> __c = ((__c >= 'a' && __c <= 'z')? ('a' - 'A') + __c : __c);
		__curr = __curr -> __next;
	}
}
 int stoi(string *s);
 double stof(string *s);
 long stol(string *s);
 long long stoll(string *s);
 string *itos(int data);
 string *ltos(long data);
 string *lltos(long long data);
 string *ftos(double data);

#endif
