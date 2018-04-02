	/* 
		 * Copyright (C) 2017 by Alexzander Purwoko Widiantoro
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
		 * @(#)NodeData.h  v4.0B @@ HEADER FILES @@
		 * Last updated in : Tuesday, March 22, 2018 on 00:34 AM
		 * Time Zone : Jakarta, GMT+7 , WIB
		 */
	/* This file is used to declare a functions on PrimitiveData.c */
 /*************************** DEFINEABLE VARIABLES **************************/
#define _ENC_
#ifdef _ENC_
#define _N_BUKA_ 0x000001
#define _N_TUTUP_ 0x000002
#define _C_BUKA_ 0x000003
#define _C_TUTUP_ 0x000004
#define _FILE_IDENT_ 0x00000f
#define _N_ID_ 0x000005
#define _ARR_ID_ 0x000006
#define _C_ID_ 0x000007
#define _LEN_ 0x000013
#define _LEN_ARR_ 0x000010
#define _LEN_SEP_ 0x000011
#define _ARR_SEP_ 0x000012
#define _DEF_TYPES_
#else
#define _N_BUKA_ '['
#define _N_TUTUP_ ']'
#define _C_BUKA_ '{'
#define _C_TUTUP_ '}'
#define _FILE_IDENT_ '@'
#define _N_ID_ '^'
#define _ARR_ID_ '>'
#define _ARR_SEP_ ','
#define _C_ID_ '<'
#define INT 'i'
#define STR 's'
#define CHR 'c'
#define DOUBLE 'f'
	// #define LDB 'F'
#define LONG 'L'
#define BOOL 'b'
#define _LEN_ '|'
#define _LEN_SEP_ '*'
#endif
#include "cstring.h"
#include "component.h"
#define __MAXBUFF__ 0x0000200	// 512 bytes
#define __NODEBUFF__ 0x0000010	// 16 bytes
#define __FILEBUFF__ 0x0000100	// 256 bytes
#define TRUE 0x1
#define FALSE 0x0
#define EDN -1
#define EDL -2
#define EENF -3
#define EFN -4
#define ECN 1
#define EUT 2
#define EIAT 3
#define EEEx 4
#define ESZ -5
#define NE 0
#define N_VAL -12
	// Structures
	struct __nodeName__
{
	char *__name;
	struct __nodeName__ *__next;
};
struct __ddatanode__
{
	void *__op1;
	void *__op2;
};
#define dnod struct __ddatanode__
typedef struct __node_data__
{
	char __temp[__MAXBUFF__];
	char __filePath[__FILEBUFF__];
	struct __nodeName__ *__lastPath;
	dnod __dop;
	dnod __fop;
	off_t __sigPos;
	off_t __lastNodeLoc;
	int __arrpos;		// for arr position
	int *__arrlen;		// for string length
	int __lenarr;		// for length array
	short __lock;		// 
	short __id;			// 
	short __errnum;
	short __idptr;
	short __gc;
} NDATA;
extern NDATA *nopen(const char *nameFile, const char *signature);
extern int nclose(NDATA * data);

extern void nadd_node(NDATA * data, const char *path, const char *node_name);
extern void ndel_node(NDATA * data, const char *fullpath);
extern void nclear_body(NDATA * data, const char *fullpath);
extern char **nlist_body(NDATA * data, const char *fullpath);

extern void nadd_data(NDATA * data, const char *path, const char *elem, const short type, void *content, short encrypt_flags);
extern void *nget_data(NDATA * data, const char *fullpath);
extern void ndel_data(NDATA * data, const char *fullpath);
extern void nset_data(NDATA * data, const char *fullpath, void *content);
extern void nstrcat_iter(NDATA * data, const char *fullpath);
extern void nnext_catc(NDATA * data, const char c);
extern void nnext_cats(NDATA * data, const char *s);
extern void nempty_data(NDATA * data, const char *fullpath);
extern short nis_empty(NDATA * data, const char *fullpath);
extern short nget_error(NDATA * data);

extern void nsetAF(NDATA * data, short decision);
extern short nisAF(NDATA * data);

extern void nadd_arr(NDATA * data, const char *path, const char *name_arr, short type, void *content, size_t size, short encrypt_flags);
extern void *nget_arr(NDATA * data, const char *fullpath);
extern size_t nget_occur(NDATA * data, const char *fullpath, void *data_comp);
extern void *nget_ap(NDATA * data, const char *fullpath, int start, int end);
extern int nget_arrlen(NDATA * data, const char *fullpath);
extern void nset_arr(NDATA * data, const char *fullpath, void *content, int size);
extern void nset_ap(NDATA * data, const char *fullpath, void *content, int size, int start, int end);
extern void nappend_arr(NDATA * data, const char *fullpath, void *content, int size);
extern char *ndisplay_error(NDATA * data);
extern void nempty_arr(NDATA * data, const char *fullpath);
extern void ndel_ap(NDATA * data, const char *fullpath, int start, int end);

 // for iteration collecting data in an array
extern void nreadarr_iter(NDATA * data, const char *fullpath);
extern void *nnext_read(NDATA * data);
extern void nfinish_read(NDATA * data);
// for iteration appending data in an array
extern void nappendarr_iter(NDATA * data, const char *fullpath);
extern void nnext_appendarr(NDATA * data, void *content);
 // for iteration adding node
extern void naddnode_iter(NDATA * data, const char *fullpath);
extern void nnext_addnode(NDATA * data, const char *node_name);
 // for iteration adding array
extern void naddarr_iter(NDATA * data, const char *path, const char *name, short type, short encrypt_flags);
extern void nnext_addarr(NDATA * data, void *content);
 // for saving the iteration
extern void nsave(NDATA * data);
 // return true if any iterator operation
extern short nisLocked(NDATA * data);
extern short nget_td(NDATA * data, const char *fullpath); // for get the data types
extern short nis_exists(NDATA *data, const char *fullpath);
#define Data NDATA *
