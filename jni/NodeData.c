
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
	 *  @(#)NodeData.c  v4.0B @@ SOURCE FILES @@ LIMITED ONLY
	 * Last updated in : Tuesday March, 22, 2018 on 00:34 AM
	 * Time Zone : Jakarta, GMT+7 , WIB
	 */
#include "NodeData.h"
#define __nname__ struct __nodeName__ *
	/* 
	 * used to define all variables in all visible function
	 * please for compatibility... don't use ! charecters
	 */
#define __DEFINE_VARS__ \
	char *_temp = data -> __temp; \
	__nname__ __lastPath = data -> __lastPath; \
	FILE *_open = (data -> __fop).__op1; \
	char *_filepath = data -> __filePath; \
	short *_errnum = &(data -> __errnum); \
	off_t *_sigPos = &(data -> __sigPos); \
	off_t *_lastNodeLoc = &(data -> __lastNodeLoc); \
	short *_lock = &(data -> __lock);
/*** THIS METHOD TO ORGANIZING THE NODE PATH TO ELEMENT *****/
	__nname__ __create_n()
{
	return (__nname__) malloc(sizeof(struct __nodeName__));
}

__nname__ __insert_n(__nname__ __to, __nname__ __s)
{
	if (!__to)
		return __s;
	__to->__next = __insert_n(__to->__next, __s);
}

__nname__ __del_n(__nname__ __to)
{
	if (!__to)
		return NULL;
	__nname__ __tmp = __to;
	__nname__ __curr = __to->__next;
	free(__tmp->__name);
	free(__tmp);
	__to = NULL;
	return __del_n(__curr);

}

int __get_length(__nname__ __s)
{
	int count = 0;
	while (__s)
	{
		__s = __s->__next;
		count++;
	}
	return count;
}

char *__get_str(__nname__ __s, int __pos)
{
	register int __x = 0;
	while (__s)
	{
		if (__x == __pos)
			return __s->__name;
		__x++;
		__s = __s->__next;
	}
	return NULL;
}

/********************* END OF METHOD FOR ORG. NODE ******************/
/********************* METHOD FOR ORG. ELEMENTS AND SWITCHING IN NODE ***************/
// Encryptor for number
int __encNum__(int x)
{
	return (2 << 3) + (x - (((1 << 5) + (1 << 3) + (1 << 1)) + ~(~0 + ~0)));
}

// Decryptor for number
int __decNum__(int x)
{
	return (2 << 3) + (x + ((1 << 3) + (1 << 1)) + ~(~0 + ~0));
}

// Encryptor for String
int __encStr__(int x)
{
	return x + ((1 << 7) - (1 << 2));
}

// Decryptor for String
int __decStr__(int x)
{
	return x - ((1 << 7) - (1 << 2));
}

/**
 * edNum( String_of_number_to_enc_or_dec, mode_dec_or_enc);
 * Encrypt or Decrypt the String of number into its string again
 * without return
 */
void __edNum__(char num[], int mode)
{
	reverse(num);
	size_t len = strlen(num), x = 0;
	register int chr;
	for (; x < len; x++)
	{
		if (mode == ENC)
			chr = __encNum__(num[x]);
		else if (mode == DEC)
			chr = __decNum__(num[x]);
		num[x] = chr;
	}
}

/**
 * edStr( String_to_enc_or_dec, mode_dec_or_enc);
 * Encrypt or Decrypt the String into its string again
 * without return
 */
void __edStr__(char *str, int mode)
{
	reverse(str);
	size_t x = 0;
	register int chr = 0;
	for (; str[x] != '\0'; x++)
	{
		if (mode == ENC)
			chr = __encStr__((int)str[x]);
		else if (mode == DEC)
			chr = __decStr__((int)str[x]);
		str[x] = (char)chr;
		chr = 0;
	}
	str[x] = '\0';
}

int __check_sig__(char *temp, FILE * f, const char *signature)
{
	register int elem, chr;
	if (f == NULL)
		return 0;
	if ((chr = getc(f)) != _FILE_IDENT_)
		return -1;
	for (elem = 0; (chr = getc(f)) != '\n' && chr != -1; elem++)
		temp[elem] = chr;
	if (chr == -1)
		return 0;
	temp[elem] = '\0';

	// printf("%d",cmp(temp,signature));
#ifdef _ENC_
	__edStr__(temp, DEC);
#endif
	int bol = cmp(temp, signature);
	if (bol)
		return elem + 2;
	else
		return 0;

}

int __write_new__(char *temp, const char *file, const char *sig)
{
	FILE *fj = fopen(file, "w");
	strcpy(temp, sig);
#ifdef _ENC_
	__edStr__(temp, ENC);
#endif
	/* int x; register int _x, _y; putc(_FILE_IDENT_, fj); for(_x = _y = 0;
	   temp[_x] != '\0'; _x++) putc(temp[_x], fj); putc('\n', fj); */
	fprintf(fj, "%c%s\n", _FILE_IDENT_, temp);
	fclose(fj);
	return strlen(temp);
}

__nname__ __con_pathNode__(const char *path)
{
	__nname__ __tmp = NULL;
	__nname__ __curr = NULL;
	register int __x = 0, __y = 0, __c;
	while ((__c = path[__x]) != '\0')
	{
		if (!__tmp)
		{
			__tmp = __create_n();
			__tmp->__name = malloc(__NODEBUFF__);
			__tmp->__next = NULL;
		}
		if (path[__x] == '/' || path[__x] == ',')
		{
			__tmp->__name[__y] = '\0';
			__curr = __insert_n(__curr, __tmp);
			__tmp = NULL;
			__y = 0;
		}
		else if (path[__x + 1] == '\0')
		{
			__tmp->__name[__y++] = __c;
			__tmp->__name[__y] = '\0';
			__curr = __insert_n(__curr, __tmp);
			__tmp = NULL;
			__y = 0;
			break;
		}
		else
			__tmp->__name[__y++] = __c;
		__x++;
	}
	return __curr;
}

off_t __select_datanode__(NDATA * data, __nname__ __node)
{
	if (!data)
		return -1;
	if (!__node)
		return -1;
	__DEFINE_VARS__ register int __pos = 0, __npos = 0, __lock = 0, __x = 0, chr;
	fseek(_open, *_sigPos, 0);
	while (__node)
	{
		if (!__lock)
		{
			// point into _N_ID_
			while ((chr = getc(_open)) != _N_ID_ && chr != _N_TUTUP_ && chr != -1);
			if (chr == -1)
			{
				// fseek(_open, *_sig, 0);
				return -1;
			}
			if (chr == _N_TUTUP_)
			{
				// fseek(_open, *_sig, 0);
				return -1;
			}
			// getting node names
			__x = 0;
			while ((chr = getc(_open)) != _N_BUKA_)
				_temp[__x++] = chr;
			_temp[__x] = '\0';
			if (chr == _N_BUKA_)
				__pos++;
			// compare nodes
			// printf("\ntmp : %s, nname : %s", _temp, __node -> __name);
			if (!strcmp(__node->__name, _temp))
				__npos++;
			// pointing into the current _N_TUTUP_ id
			if (__pos != __npos)
			{
				while (__pos != __npos)
				{
					chr = getc(_open);
					// if reaching _N_BUKA_id increment __pos
					if (chr == -1)
					{
						fseek(_open, *_sigPos, 0);
						return -1;
					}
					if (chr == _N_BUKA_)
						__pos++;
					// else if reaching _N_TUTUP_id decrement __pos
					else if (chr == _N_TUTUP_)
						__pos--;
				}
				// getc(_open);
				continue;
			}
			else
			{
				__lock = 1;
			}
		}
		else
		{
			__node = __node->__next;
			__lock = 0;
		}
	}
	// printf("\noffset current : %d, next char %c", ftell(_open),
	// getc(_open));
	off_t result = ftell(_open);
	// fseek(_open, *_sig, 0);
	return result;
}

char *__getCon_and_path__(char *__fullpath)
{
	size_t len = strlen(__fullpath);
	size_t x = len - 1;
	for (; x > 0 && __fullpath[x] != '/' && __fullpath[x] != ','; x--);
	register int _next, _y = 0;
	if (x == 0)
	{
		register int _next = '\0', _y = 0, _prev = '\0', _len = strlen(__fullpath);
		for (; _y <= _len; _y++)
		{
			_next = __fullpath[_y];
			__fullpath[_y] = _prev;
			_prev = _next;
		}
		__fullpath[_y] = '\0';
		return __fullpath + 1;
	}
	__fullpath[x] = '\0';
	return __fullpath + (++x);
}

off_t __check_and_pointE__(NDATA * __dp, __nname__ __path, const char *__content)
{
	if (!__dp)
		return 0;
	char *_temp = __dp->__temp;
	FILE *_open = (__dp->__fop).__op1;
	__nname__ __lastPath = __dp->__lastPath;
	off_t offset = 0;
	// printf("\n __lastPath %d, __path %d", __lastPath, __path);
	if (__lastPath)
	{
		if (__get_length(__lastPath) == __get_length(__path))
		{
			// printf("\n^^^^^^^^");
			while (__lastPath && __path)
			{
				int ck = strcmp(__lastPath->__name, __path->__name);
				// printf("\nck %d", ck);
				if (!ck)
				{
					// printf("\n*************");
					break;
				}
				__lastPath = __lastPath->__next;
				__path = __path->__next;
			}
			if (!__lastPath)
				offset = __dp->__lastNodeLoc;
		}
	}
	if (!__path)
	{
		offset = __dp->__sigPos;
		fseek(_open, offset, 0);
		register int __pos = 0, __c, __x = 0;
		while ((__c = getc(_open)) != -1)
		{
			if (__c == _N_BUKA_)
				__pos++;
			else if (__c == _N_TUTUP_)
				__pos--;
			if (!__pos)
			{
				if (__c == _C_ID_ || __c == _ARR_ID_)
				{
					getc(_open);
					getc(_open);
					// getting str 
					while ((__c = getc(_open)) != _LEN_)
						_temp[__x++] = __c;
					_temp[__x] = '\0';
					__x = 0;
					// printf("\ntemp : %s, content : %s", _temp, __content);
#ifdef _ENC_
					__edStr__(_temp, DEC);
#endif
					if (!strcmp(_temp, __content))
					{
						fseek(_open, ftell(_open) - strlen(__content) - 4, 0);
						return ftell(_open);
					}
				}
			}
			// printf("\nloop %d, %c", cnt++, __c);
		}
		// printf("\nuyuub\n\n");
		return 0;
	}
	if (!offset)
	{
		if (__lastPath)
		{
			// release available path
			__lastPath = __del_n(__lastPath);
			// passing current path
			__dp->__lastPath = __lastPath = __path;
			// getting pointer to offset
			offset = __select_datanode__(__dp, __lastPath);
			if (offset < 1)
				return 0;
		}
		else
		{
			// passing current path
			__dp->__lastPath = __lastPath = __path;
			// getting pointer to offset
			offset = __select_datanode__(__dp, __lastPath);
			if (offset < 1)
				return 0;
		}
	}
	// point cursor into current node
	fseek(_open, offset, 0);
	register int __x = 0, __c, __pos = 1;
	while (__pos != 0)
	{
		__c = getc(_open);
		if (__c == _N_BUKA_)
			__pos++;
		else if (__c == _N_TUTUP_)
			__pos--;
		if (__pos == 1)
			if (__c == _C_ID_ || __c == _ARR_ID_)
			{
				getc(_open);
				getc(_open);
				// getting str 
				while ((__c = getc(_open)) != _LEN_)
					_temp[__x++] = __c;
				_temp[__x] = '\0';
				__x = 0;
#ifdef _ENC_
				__edStr__(_temp, DEC);
#endif
				if (!strcmp(_temp, __content))
				{
					fseek(_open, ftell(_open) - strlen(__content) - 4, 0);
					return ftell(_open);
				}
			}

	}
	fseek(_open, ftell(_open) - 1, 0);
	return 0;
}

// find a good places for write new node or new elements
short __getPos_for_writing(NDATA * data, const char *path, const char *node_name)
{
	char *_temp = data->__temp;
	__nname__ __lastPath = data->__lastPath;
	FILE *_open = (data->__fop).__op1;
	char *_filepath = data->__filePath;
	short *_errnum = &(data->__errnum);
	off_t *_sigPos = &(data->__sigPos);
	off_t *_lastNodeLoc = &(data->__lastNodeLoc);
	register int _x = 0, _y = 0, _npos = 0;
	if (!path)
	{
		fseek(_open, *_sigPos, 0);
		if (!node_name)
			return -2;
		__nname__ __node = __con_pathNode__(node_name);

		off_t offset = __select_datanode__(data, __node);
		// printf("%d %d\n", getc(_open), offset);
		if (offset == -1)
			return 0;
		return -1;
	}
	else
	{
		// construct path node
		__nname__ __path = __con_pathNode__(path);
		off_t offset = 0;
		if (__lastPath)
		{
			if (__get_length(__lastPath) == __get_length(__path))
			{
				while (__lastPath && __path)
				{
					if (strcmp(__lastPath->__name, __path->__name))
					{
						break;
					}
					__lastPath = __lastPath->__next;
					__path = __path->__next;
				}
				if (!__lastPath)
					offset = data->__lastNodeLoc;
			}
		}

		if (!offset)
		{
			// release available path
			__lastPath = __del_n(__lastPath);
			// passing current path
			data->__lastPath = __lastPath = __path;
			// getting pointer to offset
			offset = __select_datanode__(data, __lastPath);
		}
		if (offset == -1)
			return -1;
		fseek(_open, offset, 0);
		// checking if any nodes is same
		_y = 0;
		_npos = 1;
		while (_npos != 0)
		{
			_x = getc(_open);
			if (_x == _N_ID_)
				_npos++;
			if (_x == _N_TUTUP_)
				_npos--;
			if (_npos == 2)
			{
				while ((_x = getc(_open)) != _N_BUKA_)
					_temp[_y++] = _x;
				_temp[_y] = '\0';
				_y = 0;
				if (node_name)
					if (!strcmp(_temp, node_name))
					{
						*_errnum = -1;
						return -1;
					}
			}
		}
		fseek(_open, ftell(_open) - 1, 0);
		return 1;
	}
}

 // for dinamycally allocating memory
void *__mem_alc__(NDATA * data, short type, short isArr, int size)
{
	if (!nisAF(data))
	{
		(data->__dop).__op1 = NULL;
		data->__idptr = 0;
	}
	if ((data->__dop).__op1)
		return NULL;
	register short _mem = 0;
	switch (type)
	{
	case INT:
		_mem = sizeof(int);
		break;
	case CHR:
		_mem = sizeof(char);
		break;
	case BOOL:
		_mem = sizeof(short);
		break;
	case LONG:
		_mem = sizeof(long long int);
		break;
	case DOUBLE:
		_mem = sizeof(double);
		break;
		/* case LDB: _mem = sizeof(long double); break; */
	case STR:
		{
			if (isArr)
			{
				(data->__dop).__op1 = malloc(size * sizeof(char *));
				data->__idptr = type + 10;
				return (data->__dop).__op1;
			}
			else
			{
				(data->__dop).__op1 = malloc(size);
				data->__idptr = type;
				return (data->__dop).__op1;
			}
		}
	}
	data->__idptr = type;
	(data->__dop).__op1 = malloc(_mem * size);
	return (data->__dop).__op1;
}

// release the memory allocated dinamycally
void __mem_rel__(NDATA * data)
{
	if (!data)
		return;
	if (!nisAF(data))
		return;
	switch (data->__idptr)
	{
	case INT:
	case CHR:
	case BOOL:
	case LONG:
	case DOUBLE:
		// case LDB:
	case STR:
		free((data->__dop).__op1);
		(data->__dop).__op1 = NULL;
		break;
	case STR + 10:
		{
			char **str = (char **)(data->__dop).__op1;
			register int count = 0;
			for (; str[count] != NULL; count++)
				free(str[count]);
			free(str[count]);
			free(str);
			(data->__dop).__op1 = NULL;
		}
	}
	data->__idptr = 0;
	return;
}

static __inline__ char *itoa(int __len)
{
	register int length = 0, __ln = __len;
	while (__ln != 0)
	{
		__ln /= 10;
		length++;
	}
	char *__result = malloc(length);
	sprintf(__result, "%d", __len);
	return __result;
}

#define __DECRYPT_CONTENT__(id, str) \
	switch((id)){ \
		case INT: \
		case DOUBLE: \
		case BOOL: \
		case LONG: \
			__edNum__((str), DEC); \
			break; \
		case STR: \
			__edStr__((str), DEC); \
			break; \
		case CHR: \
			_x = (str)[0]; \
			(str)[0] = __decStr__(_x); \
			break; \
	}
/********************* for array operations only ****************/
#define __ENCRYPT_DATA__(type, str) \
	switch((type)){ \
		case INT: \
		case DOUBLE: \
		case BOOL: \
		case LONG: \
			__edNum__((str), ENC);\
			break; \
		case STR: \
			__edStr__((str), ENC); \
	}
#define __PASS_SLEN__(file) \
	putc(_LEN_, (file)); \
	register int z = 0; \
	char **str = content; \
	for(_y = 0; _y < size; _y++){ \
		sprintf(_temp, "%d", strlen(str[_y])); \
		while(_temp[z] != '\0'){ \
			putc(_temp[z], (file)); \
			z++;\
		} \
		z = 0; \
		if(_y+1 != size)putc(_LEN_SEP_, (file)); \
	}

void __add_content__(FILE * _open, short *__type, char *_temp, char *__isi, void *content,
					 size_t size, short encrypt_flags)
{
	if (!content)
	{
		putc(_C_TUTUP_, _open);
		return;
	}
	register size_t x, _y, counter = 0;
	for (x = 0; x < size; x++)
	{
		switch (*__type)
		{
		case INT:
			{
				sprintf(_temp, "%d", ((int *)content)[x]);
				__isi = _temp;
			}
			break;
		case LONG:
			{
				sprintf(_temp, "%lld", ((long long int *)content)[x]);
				__isi = _temp;
			}
			break;
		case DOUBLE:
			{
				sprintf(_temp, "%lf", ((double *)content)[x]);
				__isi = _temp;
			}
			break;
		case BOOL:
			{
				sprintf(_temp, "%d", ((short *)content)[x]);
				__isi = _temp;
			}
			break;
		case CHR:
			{
				if (encrypt_flags)
					_y = __encStr__(((char *)content)[x]);
				else
					_y = (((char *)content)[x]);
				sprintf(_temp, "%c", _y);
				__isi = _temp;
			}
			break;
		case STR:
			{
				if (__isi)
					free(__isi);
				char **str = (char **)content;
				__isi = malloc(strlen(str[x]));
				strcpy(__isi, str[x]);
			}
			break;
		}
		if (encrypt_flags)
		{
			switch (*__type)
			{
			case INT:
			case DOUBLE:
			case BOOL:
			case LONG:
				__edNum__(__isi, ENC);
				break;
			case STR:
				__edStr__(__isi, ENC);
			}
		}
		// printf("\n*****After enc = %s, length : %d", __isi, strlen(__isi));
		for (counter = 0; __isi[counter] != '\0'; counter++)
			putc(__isi[counter], _open);
		if (x != size - 1)
			putc(_ARR_SEP_, _open);
		else
			putc(_C_TUTUP_, _open);
	}
	if (*__type == STR)
		if (__isi && __isi != _temp)
			free(__isi);
}

/************* END OF METHOD ORG. NODE **************/
/************* VISIBLE METHODS **************/
NDATA *nopen(const char *nameFile, const char *signature)
{
	// checking file
	char __stemp[32];
	if (checkExists(nameFile))
		__write_new__(__stemp, nameFile, signature);
	// checking signature
	// open a file mode r+
	FILE *__fp = fopen(nameFile, "r+");
	off_t offset = __check_sig__(__stemp, __fp, signature);
	if (!offset)
	{
		fclose(__fp);
		return NULL;
	}
	fseek(__fp, offset, 0);
	NDATA *__result = (NDATA *) malloc(sizeof(NDATA));
	strcpy(__result->__filePath, nameFile);
	(__result->__fop).__op1 = __fp;
	__result->__sigPos = offset;
	__result->__lenarr = __result->__arrpos = 0;
	__result->__arrlen = NULL;
	__result->__lock = -1;
	__result->__errnum = __result->__id = 0;
	__result->__lastPath = NULL;
	__result->__idptr = 0;
	__result->__gc = TRUE;
	(__result->__fop).__op2 = (__result->__dop).__op1 = (__result->__dop).__op2 = NULL;
	return __result;
}

int nclose(NDATA * data)
{
	if (!data)
		return EDN;
	nfinish_read(data);
	nsave(data);
	fclose((FILE *) ((data->__fop).__op1));
	if ((FILE *) ((data->__fop).__op2))
		fclose((FILE *) ((data->__fop).__op2));
	data->__lastPath = __del_n(data->__lastPath);
	free(data);
	return NE;
}

 // add a new node
void nadd_node(NDATA * data, const char *path, const char *node_name)
{
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__ register int _x = 0, _y = 0, _npos = 0;
	short __mode__ = __getPos_for_writing(data, path, node_name);
	// printf("%d", __mode__);
	off_t offset = ftell(_open);
	int x = getc(_open);
	// printf("\noffset : %d, char next : %c<%d>, mode : %d", offset, x, x,
	// __mode__);
	if (__mode__ == 1)
	{
		sprintf(_temp, "%s.tmp", _filepath);
		FILE *_op1 = fopen(_temp, "w+");
		// write ke file
		off_t _t = 0;
		_y = 0;
		fseek(_open, 0, 0);
		for (; _t < offset; _t++)
			putc(getc(_open), _op1);
		putc(_N_ID_, _op1);
		while ((_x = node_name[_y++]) != '\0')
			putc(_x, _op1);
		putc(_N_BUKA_, _op1);
		putc(_N_TUTUP_, _op1);
		while ((_x = getc(_open)) != -1)
			putc(_x, _op1);
		fclose(_open);
		fclose(_op1);
		rename(_temp, _filepath);
		(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	}
	else if (__mode__ == 0)
		fprintf(_open, "%c%s%c%c", _N_ID_, node_name, _N_BUKA_, _N_TUTUP_);
	else
	{
		fseek(_open, *_sigPos, 0);
		*_errnum = EENF;
		return;
	}
	fseek(_open, *_sigPos, 0);
	*_errnum = NE;
}
short nisLocked(NDATA * data)
{
	if((data->__lock)!=-1)
		return TRUE;
	else
		return FALSE;
}

void nsetAF(NDATA * data, short decision)
{
	if (!data)
		return;
	data->__gc = decision;
}

short nisAF(NDATA * data)
{
	if (!data)
		return FALSE;
	return (data->__gc) ? TRUE : FALSE;
}

// delete a nodes
void ndel_node(NDATA * data, const char *fullpath)
{
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__ register int _x = 0;
	// check if null or not
	if (!fullpath)
	{
		*_errnum = EFN;
		fseek(_open, *_sigPos, 0);
		return;
	}
	// get node 
	strcpy(_temp, fullpath);
	char *_node = __getCon_and_path__(_temp);
	int length = strlen(_node);
	// get first node position
	__nname__ __node = __con_pathNode__(fullpath);
	off_t _node_1 = __select_datanode__(data, __node);
	if (_node_1 == -1)
	{
		*_errnum = EENF;
		fseek(_open, *_sigPos, 0);
		return;
	}
	_node_1++;
	// get last node position
	__getPos_for_writing(data, fullpath, NULL);
	off_t _node_2 = ftell(_open);
	_node_1 -= (length + 3);
	fseek(_open, 0, 0);
	sprintf(_temp, "%s.tmp", _filepath);
	FILE *_op1 = fopen(_temp, "w+");
	off_t _t = 0;
	for (; _t < _node_1; _t++)
		putc(getc(_open), _op1);
	_node_2++;
	fseek(_open, _node_2, 0);
	while ((_x = getc(_open)) != -1)
		putc(_x, _op1);
	fclose(_open);
	fclose(_op1);
	rename(_temp, _filepath);
	(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	fseek(_open, *_sigPos, 0);
	*_errnum = NE;
}

// clear all content in any nodes
void nclear_body(NDATA * data, const char *fullpath)
{
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__ register int _x = 0, _c;
	// check if null or not
	if (!fullpath)
	{
		*_errnum = EFN;
		fseek(_open, *_sigPos, 0);
		return;
	}
	__nname__ __path = __con_pathNode__(fullpath);
	off_t off1 = __select_datanode__(data, __path);
	if (off1 == -1)
	{
		*_errnum = EENF;
		fseek(_open, *_sigPos, 0);
		return;
	}
	fseek(_open, off1, 0);
	_x++;
	while (_x != 0)
	{
		_c = getc(_open);
		if (_c == _N_BUKA_)
			_x++;
		else if (_c == _N_TUTUP_)
			_x--;
		else if (_c == -1)
		{
			*_errnum = ECN;
			fseek(_open, *_sigPos, 0);
			return;
		}
	}
	off_t off2 = ftell(_open);
	// printf("%d %d", off1, off2);
	fseek(_open, 0, 0);
	sprintf(_temp, "%s.tmp", _filepath);
	FILE *_op1 = fopen(_temp, "w+");
	off_t _t = 0;
	for (; _t < off1; _t++)
		putc(getc(_open), _op1);
	fseek(_open, off2 - 1, 0);
	while ((_x = getc(_open)) != -1)
		putc(_x, _op1);
	fclose(_open);
	fclose(_op1);
	rename(_temp, _filepath);
	(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	fseek(_open, *_sigPos, 0);
	*_errnum = NE;
}

// Get all contents from current path with identifier
char **nlist_body(NDATA * data, const char *fullpath)
{
	if (!data)
		return NULL;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return NULL;
	}
	__DEFINE_VARS__ register int _x = 0, _c, _npos, _y = 0;
	char **__content;
	// check if null or not
	if (!fullpath)
	{
		_npos = 0;
		while ((_c = getc(_open)) != -1)
		{
			if (_c == _N_BUKA_)
				_npos++;
			if (_c == _N_TUTUP_)
				_npos--;
			if (_npos == 0)
				if (_c == _N_ID_ || _c == _C_ID_ || _c == _ARR_ID_)
					_x++;
		}
		if (_x == 0)
		{
			fseek(_open, *_sigPos, 0);
			*_errnum = ECN;
			return NULL;
		}
		__mem_rel__(data);
		// allocate for __content
		__content = (char **)__mem_alc__(data, STR, TRUE, _x + 1);
		fseek(_open, *_sigPos, 0);
		_npos = 0;
		int sym[3];
		while ((_c = getc(_open)) != -1)
		{
			if (_c == _N_BUKA_)
				_npos++;
			if (_c == _N_TUTUP_)
				_npos--;
			if (_npos == 0)
				if (_c == _N_ID_ || _c == _C_ID_ || _c == _ARR_ID_)
				{
					_x = 0;
					if (_c == _N_ID_)
					{
						sym[0] = '(';
						_npos++;
					}
					if (_c == _C_ID_ || _c == _ARR_ID_)
					{
						if (_c == _C_ID_)
							sym[0] = '<';
						else
							sym[0] = '>';
						_c = getc(_open);
						switch (_c)
						{
						case BOOL:
							sym[1] = 'b';
							break;
						case INT:
							sym[1] = 'i';
							break;
						case CHR:
							sym[1] = 'c';
							break;
						case DOUBLE:
							sym[1] = 'F';
							break;
						case LONG:
							sym[1] = 'L';
							break;
						case STR:
							sym[1] = 's';
						}
					}
					if (sym[0] != '(')
						sym[2] = getc(_open);
					while ((_c = getc(_open)) != _N_BUKA_ && _c != _LEN_)
						_temp[_x++] = _c;
					_temp[_x] = '\0';
					// printf("\n%d, %s", _y, _temp);
					__content[_y] = malloc(strlen(_temp) + 3);
#ifdef _ENC_
					if ((_c = sym[0]) == '<' || _c == '>')
					{
						__edStr__(_temp, DEC);
					}
#endif
					if (sym[0] != '(')
						sprintf(__content[_y], "%s%c%c%c", _temp, sym[0], sym[1], sym[2]);
					else
						sprintf(__content[_y], "%s%c", _temp, sym[0]);
					_y++;
				}
		}
		__content[_y] = NULL;
		fseek(_open, *_sigPos, 0);
		*_errnum = NE;
		return __content;
	}
	__nname__ __path = __con_pathNode__(fullpath);
	off_t off1 = __select_datanode__(data, __path);
	if (off1 == -1)
	{
		*_errnum = EENF;
		fseek(_open, *_sigPos, 0);
		return NULL;
	}
	// getting length
	fseek(_open, off1, 0);
	_npos = 1;
	_x = 0;
	while (_npos != 0)
	{
		_c = getc(_open);
		if (_c == _N_BUKA_)
			_npos++;
		if (_c == _N_TUTUP_)
			_npos--;
		if (_c == -1)
			break;
		if (_npos == 1)
			if (_c == _N_ID_ || _c == _C_ID_ || _c == _ARR_ID_)
				_x++;
	}
	if (_x == 0)
	{
		fseek(_open, *_sigPos, 0);
		*_errnum = ECN;
		return NULL;
	}
	__content = (char **)__mem_alc__(data, STR, TRUE, _x + 1);
	fseek(_open, off1, 0);
	_npos = 1;
	int sym[3];
	while (_npos != 0)
	{
		_c = getc(_open);
		if (_c == _N_BUKA_)
			_npos++;
		if (_c == _N_TUTUP_)
			_npos--;
		if (_npos == 1)
			if (_c == _N_ID_ || _c == _C_ID_ || _c == _ARR_ID_)
			{
				_x = 0;
				if (_c == _N_ID_)
				{
					sym[0] = '(';
					_npos++;
				}
				if (_c == _C_ID_ || _c == _ARR_ID_)
				{
					if (_c == _C_ID_)
						sym[0] = '<';
					else
						sym[0] = '>';
					_c = getc(_open);
					switch (_c)
					{
					case BOOL:
						sym[1] = 'b';
						break;
					case INT:
						sym[1] = 'i';
						break;
					case CHR:
						sym[1] = 'c';
						break;
					case DOUBLE:
						sym[1] = 'F';
						break;
					case LONG:
						sym[1] = 'L';
						break;
					case STR:
						sym[1] = 's';
					}
				}
				if (sym[0] != '(')
					sym[2] = getc(_open);
				while ((_c = getc(_open)) != _N_BUKA_ && _c != _LEN_)
					_temp[_x++] = _c;
				_temp[_x] = '\0';
				// printf("\n%d, %s", _y, _temp);
				__content[_y] = malloc(strlen(_temp) + 3);
#ifdef _ENC_
				if ((_c = sym[0]) == '<' || _c == '>')
				{
					__edStr__(_temp, DEC);
				}
#endif
				if (sym[0] != '(')
					sprintf(__content[_y], "%s%c%c%c", _temp, sym[0], sym[1], sym[2]);
				else
					sprintf(__content[_y], "%s%c", _temp, sym[0]);
				_y++;
			}
	}
	__content[_y] = NULL;
	fseek(_open, *_sigPos, 0);
	*_errnum = NE;
	return __content;
}

 // used to add new element in specified path/nodes
void nadd_data(NDATA * data, const char *path, const char *elem, const short type, void *content,
			  short encrypt_flags)
{
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__ __nname__ __path = (path) ? __con_pathNode__(path) : NULL;
	if (__check_and_pointE__(data, __path, elem))
	{
		fseek(_open, *_sigPos, 0);
		*_errnum = EEEx;
		// printf("\n8888##$$$");
		return;
	}
	// printf("\niiiiii");
	char *__elem = malloc(strlen(elem));
	short *__type = malloc(sizeof(short));
	int *__len = malloc(sizeof(int));
	*__type = type;
	strcpy(__elem, elem);
#ifdef _ENC_
	__edStr__(__elem, ENC);
#endif
	// convert all into strings
	char *__isi;
	if (content)
	{
		switch (type)
		{
		case INT:
			{
				sprintf(_temp, "%d", (int)*((int *)content));
				__isi = _temp;
				*__len = 1;
				break;
			}
		case BOOL:
			{
				sprintf(_temp, "%d", (short)*((short *)content));
				__isi = _temp;
				*__len = 1;
				break;
			}
		case LONG:
			{
				sprintf(_temp, "%lld", (long long int)*((long long int *)content));
				__isi = _temp;
				*__len = 1;
				break;
			}
		case CHR:
			{
				if (encrypt_flags)
				{
					_temp[0] = __encStr__((char)*((char *)content));
					_temp[1] = '\0';
				}
				_temp[0] = ((char)*((char *)content));
				_temp[1] = '\0';
				__isi = _temp;
				*__len = 1;
				break;
			}
		case DOUBLE:
			{
				sprintf(_temp, "%lf", (double)*((double *)content));
				__isi = _temp;
				*__len = 1;
				break;
			}
		case STR:
			{
				*__len = strlen((char *)content);
				__isi = malloc(*__len);
				strcpy(__isi, (char *)content);
			}
			break;
		default:
			{
				*_errnum = EUT;
				fseek(_open, *_sigPos, 0);
				free(__elem);
				free(__type);
				free(__len);
				return;
			}
		}
		// printf("%s\n", __isi);
		if (encrypt_flags)
			switch (type)
			{
			case INT:
			case DOUBLE:
			case BOOL:
			case LONG:
				__edNum__(__isi, ENC);
				break;
			case STR:
				__edStr__(__isi, ENC);
			}

	}
	else
	{
		__isi = NULL;
		*__len = 0;
	}
	// writing
	off_t offset = ftell(_open);
	/* for(int ss = 0; __isi[ss] != '\0'; ss++){ printf("%d. %d\n", ss,
	   (int)__isi[ss]); } */
	if (!__path)
	{
		// printf("%s\n", (__isi)?__isi:"NULL");
		if (__isi)
			fprintf(_open, "%c%c%c%s%c%d%c%s%c", _C_ID_, type, ((encrypt_flags) ? '1' : '0'),
					__elem, _LEN_, *__len, _C_BUKA_, __isi, _C_TUTUP_);
		else
			fprintf(_open, "%c%c%c%s%c%d%c%c", _C_ID_, type, ((encrypt_flags) ? '1' : '0'), __elem,
					_LEN_, *__len, _C_BUKA_, _C_TUTUP_);
	}
	else
	{
		char __ftmp[strlen(path) + 4];
		char *__alen = itoa(*__len);
		sprintf(__ftmp, "%s.tmp", _filepath);
		FILE *_op1 = fopen(__ftmp, "w+");
		fseek(_open, 0, 0);
		off_t _t = 0;
		for (; _t < offset; _t++)
			putc(getc(_open), _op1);
		putc(_C_ID_, _op1);
		putc((char)*__type, _op1);
		putc(((encrypt_flags) ? '1' : '0'), _op1);
		register int _a = 0;
		for (; __elem[_a] != '\0'; _a++)
			putc(__elem[_a], _op1);
		putc(_LEN_, _op1);
		for (_a = 0; __alen[_a] != '\0'; _a++)
			putc(__alen[_a], _op1);
		putc(_C_BUKA_, _op1);
		_a = 0;
		// printf("%s\n", (__isi)?__isi:"NULL");
		if (__isi)
			for (; __isi[_a] != '\0'; _a++)
				putc(__isi[_a], _op1);
		putc(_C_TUTUP_, _op1);
		fseek(_open, offset, 0);
		while ((_a = getc(_open)) != -1)
			putc(_a, _op1);
		fclose(_open);
		fclose(_op1);
		rename(__ftmp, _filepath);
		(data->__fop).__op1 = _open = fopen(_filepath, "r+");
		free(__alen);
	}
	fseek(_open, *_sigPos, 0);
	if (__isi)
		*_errnum = NE;
	else
		*_errnum = N_VAL;
	if (__isi && __isi != _temp)
		free(__isi);
	free(__len);
	free(__elem);
	free(__type);
	// printf("%c %d", getc(_open), ftell(_open));
}

// getting a content value from specified path
void *nget_data(NDATA * data, const char *fullpath)
{
	// declare a variables
	if (!data)
		return NULL;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return NULL;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	// printf("%s %s\n", __path -> __name, _name);
	register int _y, _x;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return NULL;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	int *_len = malloc(sizeof(int));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type == _ARR_ID_)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return NULL;
	}
	__mem_rel__(data);
	// point into len char
	while ((_x = getc(_open)) != _LEN_);
	// getting length
	_y = 0;
	while ((_x = getc(_open)) != _C_BUKA_)
		_temp[_y++] = _x;
	_temp[_y] = '\0';
	*_len = atoi(_temp);
	// passing into temporary
	if (*_len == 0)
	{							// Null content
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = N_VAL;
		fseek(_open, *_sigPos, 0);
		return NULL;
	}
	_y = 0;
	char *__isi;
	if (*_id != STR)
	{
		while ((_x = getc(_open)) != _C_TUTUP_)
			_temp[_y++] = _x;
		_temp[_y] = '\0';
		__isi = _temp;
	}
	else
	{
		__isi = malloc(*_len);
		_y = 0;
		while ((_x = getc(_open)) != _C_TUTUP_)
			__isi[_y++] = _x;
		__isi[_y] = '\0';

	}
	if (*_en_flags)
	{
	__DECRYPT_CONTENT__(*_id, __isi)}
	// returning result
	void *__result;
	if (*_id != STR)
		__result = __mem_alc__(data, *_id, 0, *_len);
	switch (*_id)
	{
	case INT:
		{
			int *res = (int *)__result;
			*res = atoi(__isi);
		}
		break;
	case DOUBLE:
		{
			double *res = (double *)__result;
			*res = atof(__isi);
		}
		break;
	case BOOL:
		{
			short *res = (short *)__result;
			*res = ((short)atoi(__isi));
		}
		break;
	case LONG:
		{
			long long int *res = (long long int *)__result;
			*res = atoll(__isi);
		}
		break;
	case STR:
		{
			__result = __isi;
		}
		break;
	case CHR:
		{
			char *__c = malloc(sizeof(char));
			*__c = __isi[0];
			__result = __c;
		}
	}

	data->__idptr = *_id;
	(data->__dop).__op1 = __result;
	free(_id);
	free(_name);
	free(_type);
	free(_len);
	free(_en_flags);
	*_errnum = NE;
	fseek(_open, *_sigPos, 0);
	return __result;
}

// for deleting an element
void ndel_data(NDATA * data, const char *fullpath)
{
	// declare a variables
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	register int _x;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return;
	}
	// get first offset
	off_t _p1 = ftell(_open);
	// get last offset
	while ((_x = getc(_open)) != _C_TUTUP_);
	off_t _p2 = ftell(_open);
	sprintf(_temp, "%s.tmp", _filepath);
	FILE *_op1 = fopen(_temp, "w+");
	fseek(_open, 0, 0);
	off_t _t = 0;
	for (; _t < _p1; _t++)
		putc(getc(_open), _op1);
	fseek(_open, _p2, 0);
	while ((_x = getc(_open)) != -1)
		putc(_x, _op1);
	fclose(_open);
	fclose(_op1);
	rename(_temp, _filepath);
	(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	fseek(_open, *_sigPos, 0);
	*_errnum = NE;
	free(_name);
}

// change the value of an elements
void nset_data(NDATA * data, const char *fullpath, void *content)
{
	// declare a variables
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	// printf("%s %s\n", __path -> __name, _name);
	register int _y, _x;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	int *_len = malloc(sizeof(int));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type == _ARR_ID_)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return;
	}
	// convert content into _temp
	while ((_x = getc(_open)) != _LEN_);
	off_t _p0 = ftell(_open);
	_y = 0;
	while ((_x = getc(_open)) != _C_TUTUP_);
	off_t _p2 = ftell(_open);
	char *__isi;
	if (content)
	{
		switch (*_id)
		{
		case INT:
			{
				sprintf(_temp, "%d", (int)*((int *)content));
				break;
			}
		case BOOL:
			{
				sprintf(_temp, "%d", (short)*((short *)content));
				break;
			}
		case LONG:
			{
				sprintf(_temp, "%lld", (long long int)*((long long int *)content));
				break;
			}
		case CHR:
			{
				if (*_en_flags)
				{
					_temp[0] = __encStr__((char)*((char *)content));
					_temp[1] = '\0';
				}
				_temp[0] = ((char)*((char *)content));
				_temp[1] = '\0';
				break;
			}
		case DOUBLE:
			{
				sprintf(_temp, "%lf", (double)*((double *)content));
				break;
			}
		case STR:
			{
				*_len = strlen((char *)content);
				__isi = malloc(*_len);
				strcpy(__isi, (char *)content);
			}
			break;
		default:
			{
				free(_type);
				free(_id);
				free(_name);
				free(_en_flags);
				*_errnum = EUT;
				fseek(_open, *_sigPos, 0);
				return;
			}
		}
		if (*_id != STR)
		{
			__isi = _temp;
			*_len = 1;
		}
		// for encrypting
		if (*_en_flags)
			switch (*_id)
			{
			case INT:
			case DOUBLE:
			case BOOL:
			case LONG:
				__edNum__(__isi, ENC);
				break;
			case STR:
				__edStr__(__isi, ENC);
			}
	}
	else
	{
		__isi = NULL;
		*_len = 0;
	}

	char *__ftmp = malloc(strlen(_filepath) + 4);
	char *__alen = itoa(*_len);
	sprintf(__ftmp, "%s.tmp", _filepath);
	// printf("%s", __ftmp);
	FILE *_op1 = fopen(__ftmp, "w+");
	fseek(_open, 0, 0);
	off_t _t = 0;
	for (; _t < _p0; _t++)
		putc(getc(_open), _op1);
	_y = 0;
	for (; (_x = __alen[_y]) != '\0'; _y++)
		putc(_x, _op1);
	putc(_C_BUKA_, _op1);
	_y = 0;
	if (__isi)
		for (; (_x = __isi[_y]) != '\0'; _y++)
			putc(_x, _op1);
	fseek(_open, _p2 - 1, 0);
	while ((_x = getc(_open)) != -1)
		putc(_x, _op1);
	fclose(_open);
	fclose(_op1);
	rename(__ftmp, _filepath);
	(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	fseek(_open, *_sigPos, 0);
	if (*_id == STR)
		free(__isi);
	free(__alen);
	free(_len);
	free(_type);
	free(_id);
	free(_en_flags);
	free(__ftmp);
	free(_name);
	*_errnum = NE;
	return;
}

void nstrcat_iter(NDATA * data, const char *fullpath)
{
	if (!data)
		return;
	__DEFINE_VARS__ if (*_lock != -1)
		  return;
	// printf("\npos = %d", ftell(_open));
	// get the current name elements and passing into dynamic memory
	strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	// if(!__path)printf("\n__path is null");
	// printf("\npath : %s", __path -> __name);
	off_t chk = __check_and_pointE__(data, __path, _name);
	if (!chk)
	{
		fseek(_open, *_sigPos, 0);
		// printf("\n*&&&&* name %s temp %s chk %d \n", _name, _temp, chk);
		free(_name);
		*_errnum = EENF;
		return;
	}
	short _type, _id;
	_type = getc(_open);
	_id = getc(_open);
	if (_type == _ARR_ID_ || _id != STR)
	{
		free(_name);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return;
	}
	__mem_rel__(data);
	string *d = snew();
	(data->__dop).__op2 = d;
	data->__id = _id;
	*_lock = -6;
	*_errnum = NE;
	free(_name);
	return;
}

void nnext_catc(NDATA * data, const char c)
{
	if (!data)
		return;
	if ((data->__lock) != -6)
		return;
	string *d = (string *) (data->__dop).__op2;
	// printf("Added %c\n", c);
	sadd(d, c);
	return;

}

void nnext_cats(NDATA * data, const char *s)
{
	if (!data)
		return;
	if ((data->__lock) != -6)
		return;
	string *d = (string *) (data->__dop).__op2;
	register int a = 0;
	for (; s[a] != '\0'; a++)
	{
		sadd(d, s[a]);
	}
	// printf("Added %s\n", s);
	return;
}

void nempty_data(NDATA * data, const char *fullpath)
{
	nset_data(data, fullpath, NULL);
}

short nis_empty(NDATA * data, const char *fullpath)
{
	// declare a variables
	if (!data)
		return FALSE;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return FALSE;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	// printf("%s %s\n", __path -> __name, _name);
	register int _y, _x;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return FALSE;
	}
	while ((_x = getc(_open)) != _LEN_);
	_y = 0;
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_y++] = _x;
	_temp[_y] = '\0';
	free(_name);
	return ((!atoi(_temp)) ? TRUE : FALSE);
}

// display current symbol output errors
short nget_error(NDATA * data)
{
	return (data)?data->__errnum:EDN;
}

void nadd_arr(NDATA * data, const char *path, const char *name_arr, short type, void *content,
			   size_t size, short encrypt_flags)
{
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__ __nname__ __path = (path) ? __con_pathNode__(path) : NULL;
#define elem name_arr
	if (__check_and_pointE__(data, __path, elem))
	{
		fseek(_open, *_sigPos, 0);
		*_errnum = EEEx;
		return;
	}
	register int _y = 0;
	char *__elem = malloc(strlen(elem));
	short *__type = malloc(sizeof(short));
	char *__isi;
	*__type = type;
	strcpy(__elem, elem);
#ifdef _ENC_
	__edStr__(__elem, ENC);
#endif

	off_t offset = ftell(_open);
	if (!path)
	{
		if (!content)
			size = 0;
		fprintf(_open, "%c%c%d%s%c%d", _ARR_ID_, type, encrypt_flags, __elem, _LEN_, size);
		if (size != 0)
			if (*__type == STR)
			{
				__PASS_SLEN__(_open);
			}
		__isi = NULL;
		putc(_C_BUKA_, _open);
		__add_content__(_open, __type, _temp, __isi, content, size, encrypt_flags);
	}
	else
	{
		if (!content)
			size = 0;
		sprintf(_temp, "%d", size);
		char *__tpath = malloc(strlen(_filepath) + 4);
		sprintf(__tpath, "%s.tmp", _filepath);
		FILE *_op1 = fopen(__tpath, "w+");
		fseek(_open, 0, 0);
		off_t t = 0;
		for (; t < offset; t++)
			putc(getc(_open), _op1);
		// the header
		putc(_ARR_ID_, _op1);
		putc(*__type, _op1);
		putc((char)(encrypt_flags + '0'), _op1);
		_y = 0;
		for (; __elem[_y] != '\0'; _y++)
			putc(__elem[_y], _op1);
		putc(_LEN_, _op1);
		for (_y = 0; _temp[_y] != '\0'; _y++)
			putc(_temp[_y], _op1);
		if (size != 0)
			if (*__type == STR)
			{
				__PASS_SLEN__(_op1);
			}
		putc(_C_BUKA_, _op1);
		__isi = NULL;
		__add_content__(_op1, __type, _temp, __isi, content, size, encrypt_flags);
		while ((_y = getc(_open)) != -1)
			putc(_y, _op1);
		fclose(_op1);
		fclose(_open);
		rename(__tpath, _filepath);
		(data->__fop).__op1 = _open = fopen(_filepath, "r+");
		free(__tpath);
	}
	free(__elem);
	free(__type);
	fseek(_open, *_sigPos, 0);
	*_errnum = NE;
}

void *nget_arr(NDATA * data, const char *fullpath)
{
	if (!data)
		return NULL;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return NULL;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	register int _w, _x, _y, _z;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return NULL;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type != _ARR_ID_)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return NULL;
	}
	__mem_rel__(data);
	int *__arrlen = NULL;
	int *__sizearr = malloc(sizeof(int));

	// point to _LEN_
	while (getc(_open) != _LEN_);
	_y = _z = 0;
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_z++] = _x;
	_temp[_z] = '\0';
	*__sizearr = atoi(_temp);
	if (!(*__sizearr))
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		free(__sizearr);
		fseek(_open, *_sigPos, 0);
		*_errnum = N_VAL;
		return NULL;
	}
	if (*_id == STR)
	{
		_y = _z = 0;
		__arrlen = malloc(sizeof(int) * (*__sizearr));
		while ((_x = getc(_open)) != _C_BUKA_)
		{
			if (_x == _LEN_SEP_)
			{
				_temp[_y] = '\0';
				_y = 0;
				__arrlen[_z++] = atoi(_temp);
			}
			else
				_temp[_y++] = _x;
		}
		_temp[_y] = '\0';
		__arrlen[_z] = atoi(_temp);
	}
	// allocate dynamic memory
	void *_result =
		__mem_alc__(data, *_id, TRUE, ((*_id == STR) ? (*__sizearr + 1) : (*__sizearr)));
	_w = _y = _z = 0;
	// ///////// for array
	char *__tr = NULL;
	// ///////// for non - string array 
	if (*_id != STR)
		__tr = _temp;
	while (_x != _C_TUTUP_)
	{
		if (*_id == STR)
		{
			__tr = malloc(__arrlen[_w]);
		}
		_y = 0;
		while ((_x = getc(_open)) != _ARR_SEP_ && _x != _C_TUTUP_)
		{
			__tr[_y++] = _x;
		}
		__tr[_y] = '\0';
		if (*_en_flags)
		{
		__DECRYPT_CONTENT__(*_id, __tr)}
		switch (*_id)
		{
		case INT:
			{
				int *content = _result;
				content[_z++] = atoi(__tr);
				// if(_x == _C_TUTUP_)content[_z] = NULL;
			}
			break;
		case DOUBLE:
			{
				double *content = _result;
				content[_z++] = atof(__tr);
				// if(_x == _C_TUTUP_)content[_z] = NULL;
			}
			break;
		case BOOL:
			{
				short *content = _result;
				content[_z++] = (short)atoi(__tr);
				// if(_x == _C_TUTUP_)content[_z] = NULL;
			}
			break;
		case LONG:
			{
				long long int *content = _result;
				content[_z++] = atoll(__tr);
				// if(_x == _C_TUTUP_)content[_z] = NULL;
			}
			break;
		case STR:
			{
				char **content = _result;
				content[_z] = __tr;
				// strcpy(content[_z], _temp);
				_z++;
				if (_x == _C_TUTUP_)
					content[_z] = NULL;
				__tr = NULL;
			}
			break;
		case CHR:
			{
				char *content = _result;
				content[_z++] = __tr[0];
				if (_x == _C_TUTUP_)
					content[_z] = '\0';
			}
			break;
		}
		_w++;
	}
	free(__sizearr);
	if (__arrlen)
		free(__arrlen);
	free(_type);
	free(_id);
	free(_name);
	free(_en_flags);
	*_errnum = NE;
	fseek(_open, *_sigPos, 0);
	return _result;
}
size_t nget_occur(NDATA * data, const char *fullpath, void *data_comp){
	if (!data)
		return 0;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return 0;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	register int _w, _x, _y, _z;
	register size_t count_res = 0;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return 0;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type != _ARR_ID_)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return 0;
	}
	int *__arrlen = NULL;
	int *__sizearr = malloc(sizeof(int));

	// point to _LEN_
	while (getc(_open) != _LEN_);
	_y = _z = 0;
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_z++] = _x;
	_temp[_z] = '\0';
	*__sizearr = atoi(_temp);
	if (!(*__sizearr))
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		free(__sizearr);
		fseek(_open, *_sigPos, 0);
		*_errnum = N_VAL;
		return 0;
	}
	if (*_id == STR)
	{
		_y = _z = 0;
		__arrlen = malloc(sizeof(int) * (*__sizearr));
		while ((_x = getc(_open)) != _C_BUKA_)
		{
			if (_x == _LEN_SEP_)
			{
				_temp[_y] = '\0';
				_y = 0;
				__arrlen[_z++] = atoi(_temp);
			}
			else
				_temp[_y++] = _x;
		}
		_temp[_y] = '\0';
		__arrlen[_z] = atoi(_temp);
	}
	// allocate dynamic memory
	_w = _y = _z = 0;
	// ///////// for array
	char *__tr = NULL;
	// ///////// for non - string array 
	if (*_id != STR)
		__tr = _temp;
	while (_x != _C_TUTUP_)
	{
		if (*_id == STR)
		{
			__tr = malloc(__arrlen[_w]);
		}
		_y = 0;
		while ((_x = getc(_open)) != _ARR_SEP_ && _x != _C_TUTUP_)
		{
			__tr[_y++] = _x;
		}
		__tr[_y] = '\0';
		if (*_en_flags)
		{
		__DECRYPT_CONTENT__(*_id, __tr)}
		switch (*_id)
		{
		case INT:
			{
				int a = atoi(__tr);
				int dc = *((int *)data_comp);
				if(a == dc)count_res++;
			}
			break;
		case DOUBLE:
			{
				double a = atof(__tr);
				double dc = *((double *)data_comp);
				if(a == dc)count_res++;
			}
			break;
		case BOOL:
			{
				short a = (short) atoi(__tr);
				short dc = (short) *((short *)data_comp);
				if(a == dc)count_res++;
			}
			break;
		case LONG:
			{
				long long int a = atoll(__tr);
				long long int dc = *((long long int *)data_comp);
				if(a == dc)count_res++;
			}
			break;
		case STR:
			{
				char *dc = (char *)data_comp;
				if(!strcmp(dc, __tr))count_res++;
				free(__tr);
				__tr = NULL;
			}
			break;
		case CHR:
			{
				int dc = *((char *)data_comp);
				if(__tr[0] == dc)count_res++;
			}
			break;
		}
		_w++;
	}
	free(__sizearr);
	if (__arrlen)
		free(__arrlen);
	free(_type);
	free(_id);
	free(_name);
	free(_en_flags);
	*_errnum = NE;
	fseek(_open, *_sigPos, 0);
	return count_res;
}
void *nget_ap(NDATA * data, const char *fullpath, int start, int end)
{
	if (!data)
		return NULL;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return NULL;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	register int _v, _w, _x, _y, _z;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return NULL;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type != _ARR_ID_)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return NULL;
	}
	__mem_rel__(data);
	int *__arrlen = NULL;
	int *__sizearr = malloc(sizeof(int));

	// point to _LEN_
	while (getc(_open) != _LEN_);
	_y = _z = 0;
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_z++] = _x;
	_temp[_z] = '\0';
	*__sizearr = atoi(_temp);
	if (!(*__sizearr))
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		free(__sizearr);
		fseek(_open, *_sigPos, 0);
		*_errnum = N_VAL;
		return NULL;
	}
	if (*_id == STR)
	{
		_y = _z = 0;
		__arrlen = malloc(sizeof(int) * (*__sizearr));
		while ((_x = getc(_open)) != _C_BUKA_)
		{
			if (_x == _LEN_SEP_)
			{
				_temp[_y] = '\0';
				_y = 0;
				__arrlen[_z++] = atoi(_temp);
			}
			else
				_temp[_y++] = _x;
		}
		_temp[_y] = '\0';
		__arrlen[_z] = atoi(_temp);
	}
	if (start > end || end >= *__sizearr)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		free(__sizearr);
		if (__arrlen)
			free(__arrlen);
		*_errnum = ESZ;
		fseek(_open, *_sigPos, 0);
		return NULL;
	}
	// allocate dynamic memory
	void *_result;
	if (start == end)
		_result = __mem_alc__(data, *_id, FALSE, ((*_id == STR) ? __arrlen[start] : 1));
	else
		_result =
			__mem_alc__(data, *_id, TRUE, ((*_id == STR) ? (end - start) + 2 : (end - start) + 1));
	_v = _w = _x = _y = _z = 0;
	// getting the content with the location
	// ///////// for array
	char *__tr = NULL;
	// ///////// for non - string array 
	if (*_id != STR)
		__tr = _temp;
	while (_x != _C_TUTUP_)
	{
		if (_v < start)
		{
			while ((_x = getc(_open)) != _ARR_SEP_ && _x != _C_TUTUP_);
			_v++;
			continue;
		}
		if (*_id == STR)
		{
			__tr = malloc(__arrlen[_w]);
		}
		_y = 0;
		while ((_x = getc(_open)) != _ARR_SEP_ && _x != _C_TUTUP_)
		{
			__tr[_y++] = _x;
		}
		__tr[_y] = '\0';
		if (*_en_flags)
		{
		__DECRYPT_CONTENT__(*_id, __tr)}
		switch (*_id)
		{
		case INT:
			{
				int *content = _result;
				content[_z++] = atoi(__tr);
				// if(_x == _C_TUTUP_)content[_z] = NULL;
			}
			break;
		case DOUBLE:
			{
				double *content = _result;
				content[_z++] = atof(__tr);
				// if(_x == _C_TUTUP_)content[_z] = NULL;
			}
			break;
		case BOOL:
			{
				short *content = _result;
				content[_z++] = (short)atoi(__tr);
				// if(_x == _C_TUTUP_)content[_z] = NULL;
			}
			break;
		case LONG:
			{
				long long int *content = _result;
				content[_z++] = atoll(__tr);
				// if(_x == _C_TUTUP_)content[_z] = NULL;
			}
			break;
		case STR:
			{
				char **content = _result;
				content[_z] = __tr;
				// strcpy(content[_z], _temp);
				_z++;
				__tr = NULL;
			}
			break;
		case CHR:
			{
				char *content = _result;
				content[_z++] = __tr[0];
			}
			break;
		}
		_w++;
		if (_v++ == end)
		{
			if (*_id == STR)
			{
				char **content = _result;
				content[_z] = NULL;
			}
			else if (*_id == CHR)
			{
				char *content = _result;
				content[_z] = '\0';
			}
			break;
		}
	}
	free(_type);
	free(_id);
	free(_en_flags);
	free(_name);
	free(__sizearr);
	if (__arrlen)
		free(__arrlen);
	*_errnum = NE;
	fseek(_open, *_sigPos, 0);
	return _result;
}

void nset_arr(NDATA * data, const char *fullpath, void *content, int size)
{
	// declare a variables
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type != _ARR_ID_)
	{
		free(_type);
		free(_id);
		free(_name);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return;
	}
	register int _x, _y, _z;
	// passing array length
	if (!content)
		size = 0;
	int *_len = malloc(sizeof(int));
	int *_arrlen = NULL;
	*_len = size;
	if (*_len && *_id == STR)
	{
		_arrlen = malloc(sizeof(int) * size);
		for (_x = 0, _y = 0; _x < size; _x++)
			_arrlen[_y] = strlen(((char **)content)[_y++]);
	}
	// /////////
	// point into _LEN_
	while (getc(_open) != _LEN_);
	// save offset point 1
	off_t _p1 = ftell(_open);
	// point into _C_TUTUP_
	while (getc(_open) != _C_TUTUP_);
	// save offset point 2
	off_t _p2 = ftell(_open);
	// point into 0 offset 
	fseek(_open, 0, 0);
	// create temp file and open it
	char *_ftmp = malloc(strlen(_filepath) + 4);
	sprintf(_ftmp, "%s.tmp", _filepath);
	FILE *_fop = fopen(_ftmp, "w+");
	// pass content from 0 offset into last _LEN_
	off_t _op3 = 0;
	for (; _op3 < _p1; _op3++)
		putc(getc(_open), _fop);
	// pass array length for identification
	_x = _y = _z = 0;
	sprintf(_temp, "%d", *_len);
	_z = strlen(_temp);
	for (; _x < _z; _x++)
		putc(_temp[_x], _fop);
	if (*_len && *_id == STR)
	{
		_x = _z = 0;
		putc(_LEN_, _fop);
		for (; _x < *_len; _x++)
		{
			sprintf(_temp, "%d", _arrlen[_x]);
			_z = strlen(_temp);
			for (; _y < _z; _y++)
				putc(_temp[_y], _fop);
			if ((_x + 1) != *_len)
				putc(_LEN_SEP_, _fop);
			_y = _z = 0;
		}
		_x = _y = _z = 0;
	}
	// passing content of array
	putc(_C_BUKA_, _fop);
	__add_content__(_fop, _id, _temp, NULL, content, *_len, *_en_flags);
	// point into second last offset
	fseek(_open, _p2, 0);
	// pass every character until -1
	while ((_x = getc(_open)) != -1)
		putc(_x, _fop);
	// close all files
	fclose(_fop);
	fclose(_open);
	// rename and open old files
	rename(_ftmp, _filepath);
	(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	fseek(_open, *_sigPos, 0);
	// release any dynamic memory
	if (*_len && *_id == STR)
		free(_arrlen);
	free(_len);
	free(_type);
	free(_id);
	free(_en_flags);
	free(_ftmp);
	free(_name);
	*_errnum = NE;
	return;
}

void nset_ap(NDATA * data, const char *fullpath, void *content, int size, int start,
					int end)
{
	// declare a variables
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return;
	}
	if (!content)
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = N_VAL;
		return;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type != _ARR_ID_ || start > end || ((end - start) + 1) != size)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return;
	}
	register int _v, _w, _x, _y, _z;
	register short _lck = 0;
	int *_len = malloc(sizeof(int));
	int *_arrlen = NULL;
	_y = _z = 0;
	while (getc(_open) != _LEN_);
	// declare a position
	off_t _p0, _p1;
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_y++] = _x;
	_temp[_y] = '\0';
	*_len = atoi(_temp);
	if (((end - start) + 1) > *_len)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		free(_len);
		*_errnum = ESZ;
		fseek(_open, *_sigPos, 0);
		return;
	}
	_p0 = ftell(_open);
	if (*_id == STR)
	{
		_x = _y = _z = 0;
		_arrlen = malloc(sizeof(int) * (*_len));
		while ((_x = getc(_open)) != _C_BUKA_)
		{
			if (_x == _LEN_SEP_)
			{
				_temp[_y] = '\0';
				_y = 0;
				_arrlen[_z++] = atoi(_temp);
			}
			else
				_temp[_y++] = _x;
		}
		_temp[_y] = '\0';
		_arrlen[_z] = atoi(_temp);
		// change the length of each element in content
		for (_x = start, _y = _z = 0; _x <= end; _x++)
			_arrlen[_x] = strlen(((char **)content)[_y++]);
		_x = _y = _z = 0;
	}
	// open temporary files
	char *_ftmp = malloc(strlen(_filepath) + 4);
	sprintf(_ftmp, "%s.tmp", _filepath);
	FILE *_fop = fopen(_ftmp, "w+");
	// starts writing until offset occurs
	fseek(_open, 0, 0);
	for (_p1 = 0; _p1 < _p0; _p1++)
		putc(getc(_open), _fop);
	// write the length of each content string of array
	if (*_id == STR)
	{
		_w = _x = _y = _z = 0;
		for (; _w < *_len; _w++)
		{
			sprintf(_temp, "%d", _arrlen[_w]);
			for (; (_y = _temp[_x]) != '\0'; _x++)
				putc(_y, _fop);
			_x = 0;
			if ((_w + 1) != *_len)
				putc(_LEN_SEP_, _fop);
		}
		while (getc(_open) != _C_BUKA_);
		putc(_C_BUKA_, _fop);
		_w = _x = _y = _z = 0;
	}
	// write the content
	_v = _w = _x = _y = _z = 0;
	char *__isi = NULL;
	while (TRUE)
	{
		_w = getc(_open);
		if (_w == _ARR_SEP_ || _w == _C_TUTUP_ || _w == -1)
		{
			if (_x != start)
				_x++;
			if (_y != (end + 1))
				_y++;
			_lck = 0;
		}
		if (_x != start)
			putc(_w, _fop);

		if (_x == start && _y != (end + 1))
		{
			if (!_lck)
			{
				if (_w == _ARR_SEP_)
					putc(_w, _fop);

				switch (*_id)
				{
				case INT:
					{
						int *result = content;
						sprintf(_temp, "%d", result[_z]);
						__isi = _temp;
					}
					break;
				case DOUBLE:
					{
						double *result = content;
						sprintf(_temp, "%lf", result[_z]);
						__isi = _temp;
					}
					break;
				case BOOL:
					{
						short *result = content;
						sprintf(_temp, "%d", result[_z]);
						__isi = _temp;
					}
					break;
				case LONG:
					{
						long long int *result = content;
						sprintf(_temp, "%lld", result[_z]);
						__isi = _temp;
					}
					break;
				case STR:
					{
						char **astr = content;
						char *str = astr[_z];
						if (__isi && __isi != _temp)
							free(__isi);
						__isi = malloc(strlen(str));
						strcpy(__isi, str);
					}
					break;
				case CHR:
					{
						char *result = content;
						if (*_en_flags)
							_temp[0] = __encStr__(result[_z]);
						else
							_temp[0] = result[_z];
						_temp[1] = '\0';
						__isi = _temp;
					}
					break;
				}
				if (*_en_flags)
				{
				__ENCRYPT_DATA__(*_id, __isi)}
				// write !!
				for (_v = 0; __isi[_v] != '\0'; _v++)
					putc(__isi[_v], _fop);
				_z++;
				_lck = 1;
			}
		}
		if (_y == (end + 1) && _w != _C_TUTUP_)
			putc(_w, _fop);

		if (_w == _C_TUTUP_)
		{
			if (__isi && __isi != _temp)
				free(__isi);
			putc(_w, _fop);
			break;
		}
	}
	while ((_w = getc(_open)) != -1)
		putc(_w, _fop);
	fclose(_fop);
	fclose(_open);
	rename(_ftmp, _filepath);
	(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	*_errnum = NE;
	fseek(_open, *_sigPos, 0);
	free(_type);
	free(_id);
	free(_en_flags);
	free(_name);
	free(_len);
	free(_arrlen);
	free(_ftmp);
	return;
}

void nappend_arr(NDATA * data, const char *fullpath, void *content, int size)
{
	// declare a variables
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return;
	}
	if (!content)
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = N_VAL;
		return;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type != _ARR_ID_ || size < 0)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return;
	}
	register int _x, _y, _z;
	register short _lck = 0;
	int *_len = malloc(sizeof(int));
	int *_arrlen = NULL;
	_y = _z = 0;
	while (getc(_open) != _LEN_);
	// declare a position
	off_t _p0 = ftell(_open), _p1;
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_y++] = _x;
	_temp[_y] = '\0';
	*_len = atoi(_temp);
	if (*_id == STR)
	{
		_x = _y = _z = 0;
		_arrlen = malloc(sizeof(int) * ((*_len) + size));
		while ((_x = getc(_open)) != _C_BUKA_)
		{
			if (_x == _LEN_SEP_)
			{
				_temp[_y] = '\0';
				_y = 0;
				_arrlen[_z++] = atoi(_temp);
			}
			else
				_temp[_y++] = _x;
		}
		_temp[_y] = '\0';
		_arrlen[_z++] = atoi(_temp);
		for (_x = 0; _x < size; _x++)
			_arrlen[_z++] = strlen(((char **)content)[_x]);
	}
	char *_ftmp = malloc(strlen(_filepath) + 4);
	sprintf(_ftmp, "%s.tmp", _filepath);
	FILE *_fop = fopen(_ftmp, "w+");
	fseek(_open, 0, 0);
	for (_p1 = 0; _p1 < _p0; _p1++)
		putc(getc(_open), _fop);
	// passing length of array
	sprintf(_temp, "%d", (*_len + size));
	_y = 0;
	while ((_x = _temp[_y++]) != '\0')
		putc(_x, _fop);
	// passing str len
	if (*_id == STR)
	{
		putc(_LEN_, _fop);
		_x = size + *_len;
		for (_y = _z = 0; _y < _x; _y++)
		{
			sprintf(_temp, "%d", _arrlen[_y]);
			while (_temp[_z] != '\0')
				putc(_temp[_z++], _fop);
			_z = 0;
			if (_y + 1 != _x)
				putc(_LEN_SEP_, _fop);
		}
	}
	putc(_C_BUKA_, _fop);
	// point _open into _C_BUKA_
	while (getc(_open) != _C_BUKA_);
	// put any content before _C_TUTUP_ into _fop
	while ((_x = getc(_open)) != _C_TUTUP_)
		putc(_x, _fop);
	char *__isi = NULL;
	for (_x = _y = _z = 0; _x < size; _x++)
	{
		putc(_ARR_SEP_, _fop);
		switch (*_id)
		{
		case INT:
			{
				int *result = content;
				sprintf(_temp, "%d", result[_z]);
				__isi = _temp;
			}
			break;
		case DOUBLE:
			{
				double *result = content;
				sprintf(_temp, "%lf", result[_z]);
				__isi = _temp;
			}
			break;
		case BOOL:
			{
				short *result = content;
				sprintf(_temp, "%d", result[_z]);
				__isi = _temp;
			}
			break;
		case LONG:
			{
				long long int *result = content;
				sprintf(_temp, "%lld", result[_z]);
				__isi = _temp;
			}
			break;
		case STR:
			{
				char **astr = content;
				char *str = astr[_z];
				if (__isi && __isi != _temp)
					free(__isi);
				__isi = malloc(strlen(str));
				strcpy(__isi, str);
			}
			break;
		case CHR:
			{
				char *result = content;
				if (*_en_flags)
					_temp[0] = __encStr__(result[_z]);
				else
					_temp[0] = result[_z];
				_temp[1] = '\0';
				__isi = _temp;
			}
			break;
		}
		if (*_en_flags)
		{
		__ENCRYPT_DATA__(*_id, __isi)}
		// write !!
		for (_y = 0; __isi[_y] != '\0'; _y++)
			putc(__isi[_y], _fop);
		_z++;
	}
	if (__isi && __isi != _temp)
		free(__isi);
	putc(_C_TUTUP_, _fop);
	while ((_x = getc(_open)) != -1)
		putc(_x, _fop);
	fclose(_fop);
	fclose(_open);
	rename(_ftmp, _filepath);
	(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	*_errnum = NE;
	fseek(_open, *_sigPos, 0);
	free(_type);
	free(_id);
	free(_name);
	free(_en_flags);
	free(_len);
	free(_arrlen);
	free(_ftmp);
	return;
}

short nget_td(NDATA * data, const char *fullpath)
{
	if (!data)
		return 0;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return 0;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	register int _x, _z;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return 0;
	}
	getc(_open);
	short tp = getc(_open);
	free(_name);
	fseek(_open, *_sigPos, 0);
	return tp;
}
short nis_exists(NDATA *data, const char *fullpath){
	if (!data)
		return 0;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return 0;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	register int _x, _z;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return 0;
	}
	free(_name);
	fseek(_open, *_sigPos, 0);
	return 1;
}
int nget_arrlen(NDATA * data, const char *fullpath)
{
	if (!data)
		return 0;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return 0;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	register int _x, _z;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return 0;
	}
	if (getc(_open) != _ARR_ID_)
	{
		free(_name);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return 0;
	}
	// point to _LEN_
	while (getc(_open) != _LEN_);
	_z = 0;
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_z++] = _x;
	_temp[_z] = '\0';
	int __sizearr = atoi(_temp);
	free(_name);
	fseek(_open, *_sigPos, 0);
	return __sizearr;
}
void ndel_ap(NDATA * data, const char *fullpath, int start, int end)
{
	// declare a variables
	if (!data)
		return;
	if (nisLocked(data))
	{
		data->__errnum = EDL;
		return;
	}
	__DEFINE_VARS__
		// get the current name elements and passing into dynamic memory
		strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return;
	}
	off_t _p0, _p1;
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	*_type = getc(_open);
	*_id = getc(_open);
	getc(_open);				// encrypt_flags not used
	register int _v, _w, _x, _y, _z;
	register short _lck = 0;
	int *_len = malloc(sizeof(int));
	int *_arrlen = NULL;
	_y = _z = 0;
	while (getc(_open) != _LEN_);
	_p0 = ftell(_open);
	// declare a position
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_y++] = _x;
	_temp[_y] = '\0';
	*_len = atoi(_temp);
	if (*_type != _ARR_ID_ || start > end || ((end - start) + 1) == *_len)
	{
		free(_type);
		free(_id);
		free(_name);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return;
	}
	if (((end - start) + 1) > *_len || end >= *_len)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_len);
		*_errnum = ESZ;
		fseek(_open, *_sigPos, 0);
		return;
	}
	_lck = *_len;
	*_len -= ((end - start) + 1);
	if (*_id == STR)
	{
		_w = _x = _y = _z = 0;

		_arrlen = malloc(sizeof(int) * (*_len));
		while (_x != _C_BUKA_)
		{
			if (_w <= end)
				if (_w >= start)
				{
					while ((_x = getc(_open)) != _LEN_SEP_);
					_w++;
					continue;
				}

			_y = 0;
			while ((_x = getc(_open)) != _LEN_SEP_ && _x != _C_BUKA_)
				_temp[_y++] = _x;
			_temp[_y] = '\0';
			_arrlen[_z++] = atoi(_temp);
			_w++;
		}
		_w = _x = _y = _z = 0;
	}
	// open temporary files
	char *_ftmp = malloc(strlen(_filepath) + 4);
	sprintf(_ftmp, "%s.tmp", _filepath);
	FILE *_fop = fopen(_ftmp, "w+");
	// starts writing until offset occurs
	fseek(_open, 0, 0);
	for (_p1 = 0; _p1 < _p0; _p1++)
		putc(getc(_open), _fop);
	sprintf(_temp, "%d%c", *_len, (*_id == STR)?_LEN_: _C_BUKA_);
	for (_w = 0; _temp[_w]!='\0'; _w++)
		putc(_temp[_w], _fop);
	
	// write the length of each content string of array
	if (*_id == STR)
	{
		_w = _x = _y = _z = 0;
		for (; _w < *_len; _w++)
		{
			sprintf(_temp, "%d", _arrlen[_w]);
			for (; (_y = _temp[_x]) != '\0'; _x++)
				putc(_y, _fop);
			_x = 0;
			if ((_w + 1) != *_len)
				putc(_LEN_SEP_, _fop);
		}
		while (getc(_open) != _C_BUKA_);
		putc(_C_BUKA_, _fop);
		_w = _x = _y = _z = 0;
	}
	else 
		while (getc(_open) != _C_BUKA_);
	// write the content
	_v = _w = _x = _y = _z = 0;
	char *__isi = NULL;
	while (_x != _C_TUTUP_)
	{
		if (_w <= end)
			if (_w >= start)
			{
				while ((_x = getc(_open)) != _ARR_SEP_ && _x != _C_TUTUP_);
				_w++;
				continue;
			}
		_y = 0;
		while ((_x = getc(_open)) != _ARR_SEP_ && _x != _C_TUTUP_)
			putc(_x, _fop);
		_w++;
		if (_w == start && end == (_lck - 1))
			putc(_C_TUTUP_, _fop);
		else
			putc(_x, _fop);
	}
	while ((_w = getc(_open)) != -1)
		putc(_w, _fop);
	fclose(_fop);
	fclose(_open);
	rename(_ftmp, _filepath);
	(data->__fop).__op1 = _open = fopen(_filepath, "r+");
	*_errnum = NE;
	fseek(_open, *_sigPos, 0);
	free(_type);
	free(_id);
	free(_name);
	free(_len);
	free(_arrlen);
	free(_ftmp);
	return;
}
void nempty_arr(NDATA * data, const char *fullpath)
{
	nset_arr(data, fullpath, NULL, 0);
}
char *ndisplay_error(NDATA * data)
{
	if(!data)return "Error : Data is null, You must not using this variable";
	switch (data->__errnum)
	{
	case EDN:
		return "Error : Data is null, You must not using this variable";
	case EDL:
		return
			"Error : (locked Data)You can't use any method, because Your data is locked by another process. You can fix this by calling resetPoint(data) method";
	case EENF:
		return "Error : Your name element is not found in node or in data file";
	case EFN:
		return "Error : fullpath parameters is null";
	case ECN:
		return "Error : Content in node is NULL";
	case EUT:
		return "Error : Undefined Type Data";
	case EIAT:
		return "Error : Invalid Array Type Data Or invalid value of parameter start and end.";
	case EEEx:
		return "Error : Name Element of data is Exists.";
	case ESZ:
		return "Error : Size is overflow!";
	case NE:
		return "No Error Occured!";
	case N_VAL:
		return "Warning : Nullable Data Values!";
	default:
		return "Error is undefined, I'm sorry";
	}
}
void nreadarr_iter(NDATA * data, const char *fullpath)
{
	// declare a variables
	if (!data)return;
	__DEFINE_VARS__ 
	if (*_lock != -1)return;
	// get the current name elements and passing into dynamic memory
	strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	if (!__check_and_pointE__(data, __path, _name))
	{
		fseek(_open, *_sigPos, 0);
		free(_name);
		*_errnum = EENF;
		return;
	}
	short *_type = malloc(sizeof(short));
	short *_id = malloc(sizeof(short));
	short *_en_flags = malloc(sizeof(short));
	*_type = getc(_open);
	*_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (*_type != _ARR_ID_)
	{
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return;
	}
	register int _x, _y, _z;
	while (getc(_open) != _LEN_);
	_y = _z = 0;
	while ((_x = getc(_open)) != _LEN_ && _x != _C_BUKA_)
		_temp[_y++] = _x;
	_temp[_y] = '\0';
	data->__arrpos = 0;
	data->__lenarr = atoi(_temp);
	if(data -> __lenarr < 1){
		data -> __lenarr = 0;
		free(_type);
		free(_id);
		free(_name);
		free(_en_flags);
		*_errnum = ECN;
		fseek(_open, *_sigPos, 0);
		return;
	}
	if (*_id == STR)
	{
		_x = _y = _z = 0;
		int *_arrlen = (data->__arrlen = malloc(sizeof(int) * (data->__lenarr)));
		while ((_x = getc(_open)) != _C_BUKA_)
		{
			if (_x == _LEN_SEP_)
			{
				_temp[_y] = '\0';
				_y = 0;
				_arrlen[_z++] = atoi(_temp);
			}
			else
				_temp[_y++] = _x;
		}
		_temp[_y] = '\0';
		_arrlen[_z++] = atoi(_temp);
	}
	*_lock = 0;
	*_errnum = NE;
	data->__id = *_id;
	(data -> __dop).__op2 = _en_flags;
	free(_type);
	free(_id);
	free(_name);
	return;
}

void *nnext_read(NDATA * data)
{
	if (!data)
		return NULL;
	__DEFINE_VARS__ 
	if (*_lock != 0)
		  return NULL;
	register int _x, _y, _z = 0;
	__mem_rel__(data);
	short *_en_flags = (short *) (data -> __dop).__op2;
	if (data->__id == STR)
	{
		_y = 0;
		char *_result = __mem_alc__(data, data->__id, FALSE, data->__arrlen[data->__arrpos]);
		while ((_x = getc(_open)) != _ARR_SEP_ && _x != _C_TUTUP_){
			_result[_y++] = _x;
		}
		_result[_y] = '\0';
		if(*_en_flags){
			__edStr__(_result, DEC);
		}
		if (_x == _C_TUTUP_)
			*_lock = -2;
		data->__arrpos = data->__arrpos + 1;
		*_errnum = NE;
		return _result;
	}
	else
	{
		_y = 0;
		void *_result = __mem_alc__(data, data->__id, FALSE, 1);
		while ((_x = getc(_open)) != _ARR_SEP_ && _x != _C_TUTUP_)
			_temp[_y++] = _x;
		_temp[_y] = '\0';
		if (_x == _C_TUTUP_)
			*_lock = -2;
		if(*_en_flags){
			__DECRYPT_CONTENT__(data->__id, _temp)
		}
			_x = 0;
		switch (data->__id)
		{
		case INT:
			{
				int *content = _result;
				*content = atoi(_temp);
			}
			break;
		case DOUBLE:
			{
				double *content = _result;
				*content = atof(_temp);
			}
			break;
		case BOOL:
			{
				short *content = _result;
				*content = (short)atoi(_temp);
			}
			break;
		case LONG:
			{
				long long int *content = _result;
				*content = atoll(_temp);
			}
			break;
		case CHR:
			{
				char *content = _result;
				*content = _temp[0];
			}
			break;
		}
		*_errnum = NE;
		data->__arrpos = data->__arrpos + 1;
		return _result;
	}
	return NULL;
}

void nfinish_read(NDATA * data)
{
	if (!data)
		return;
	switch (data->__lock)
	{
	case -1:
	case -3:
	case -4:
	case -5:
	case -6:
		return;
	}
	data->__arrpos = 0;
	if (data->__id == STR && data->__arrlen)
		free(data->__arrlen);
	data->__arrlen = NULL;
	data->__lenarr = 0;
	data->__lock = -1;
	data->__id = 0;
	free((data -> __dop).__op2);
	(data -> __dop).__op2 = NULL;
	fseek((data->__fop).__op1, data->__sigPos, 0);
}

void naddnode_iter(NDATA * data, const char *path)
{
	if (!data)
		return;
	__DEFINE_VARS__ 
	if (*_lock != -1)
		  return;
	register int _x = 0, _y = 0, _npos = 0;
	short __mode__ = __getPos_for_writing(data, path, NULL);
	// printf("%d", __mode__);
	off_t offset = ftell(_open);
	if (__mode__ != -1)
	{
		sprintf(_temp, "%s.tmp", _filepath);
		FILE *_op1 = fopen(_temp, "w+");
		if (__mode__ == -2)
		{
			fseek(_open, 0, 0);
			while ((_x = getc(_open)) != -1)
				putc(_x, _op1);
			(data->__fop).__op2 = _op1;
			data->__lock = -3;
			data->__errnum = NE;
			return;
		}
		off_t _t = 0;
		_y = 0;
		fseek(_open, 0, 0);
		for (; _t < offset; _t++)
			putc(getc(_open), _op1);
		(data->__fop).__op2 = _op1;
		data->__lock = -3;
		data->__errnum = NE;
		// free(data -> __tmp);
		return;
	}
	else
	{
		fseek(_open, *_sigPos, 0);
		*_errnum = EENF;
		return;
	}
}

void nnext_addnode(NDATA * data, const char *node_name)
{
	if (!data)
		return;
	__DEFINE_VARS__ if (*_lock != -3)
		  return;
	FILE *tmp = (data->__fop).__op2;
	putc(_N_ID_, tmp);
	register int x, y;
	for (x = 0; (y = node_name[x]) != '\0'; x++)
		putc(y, tmp);
	putc(_N_BUKA_, tmp);
	putc(_N_TUTUP_, tmp);
	*_errnum = NE;
}
void nappendarr_iter(NDATA * data, const char *fullpath)
{
	if (!data)
		return;
	__DEFINE_VARS__ 
	if (*_lock != -1)
		  return;
	// printf("\npos = %d", ftell(_open));
	// get the current name elements and passing into dynamic memory
	strcpy(_temp, fullpath);
	char *__name = __getCon_and_path__(_temp);
	char *_name = malloc(strlen(__name));
	strcpy(_name, __name);
	__nname__ __path = (_temp[0] != '\0') ? __con_pathNode__(_temp) : NULL;
	off_t chk = __check_and_pointE__(data, __path, _name);
	if (!chk)
	{
		fseek(_open, *_sigPos, 0);
		// printf("\n*&&&&* name %s temp %s chk %d \n", _name, _temp, chk);
		free(_name);
		*_errnum = EENF;
		return;
	}
	short _type, _id, *_en_flags = (short *)malloc(sizeof(short));
	_type = getc(_open);
	_id = getc(_open);
	*_en_flags = getc(_open) - '0';
	if (_type != _ARR_ID_)
	{
		free(_name);
		*_errnum = EIAT;
		fseek(_open, *_sigPos, 0);
		return;
	}
	__mem_rel__(data);
	while (getc(_open) != _LEN_);
	// opening a file for appending
	//void *_tmpFile = __mem_alc__(data, STR, FALSE, strlen(_filepath) + 4);
	sprintf(_temp, "%s.tmp", _filepath);
	FILE *_tmp = fopen(_temp, "w+");
	off_t pos = ftell(_open), count = 0;
	fseek(_open, 0, 0);
	for (; count < pos; count++)
		putc(getc(_open), _tmp);
	(data->__fop).__op2 = _tmp;
	(data->__dop).__op1 = _en_flags;
	data->__id = _id;
	*_lock = -4;
	*_errnum = NE;
	free(_name);
	return;
}

void nnext_appendarr(NDATA * data, void *content)
{
	if (!data)
		return;
	__DEFINE_VARS__ 
	if (*_lock != -4)
		  return;
	register int x, y;
	char *__isi;
	short *_en_flags = (short *) (data->__dop).__op1;
	switch (data->__id)
	{
		case INT:
		{
			int *result = content;
			sprintf(_temp, "%d", *result);
			__isi = _temp;
		}
		break;
		case DOUBLE:
		{
			double *result = content;
			sprintf(_temp, "%lf", *result);
			__isi = _temp;
		}
		break;
		case BOOL:
		{
			short *result = content;
			sprintf(_temp, "%d", *result);
			__isi = _temp;
		}
		break;
		case LONG:
		{
			long long int *result = content;
			sprintf(_temp, "%lld", *result);
			__isi = _temp;
		}
		break;
		case STR:
		{
			char *str = content;
			__isi = malloc(strlen(str));
			strcpy(__isi, str);
		}
		break;
		case CHR:
		{
			char *result = content;
			if(*_en_flags)
				_temp[0] = __encStr__(*result);
			else
				_temp[0] = *result;
			_temp[1] = '\0';
			__isi = _temp;
		}
	}
	if(*_en_flags){
		__ENCRYPT_DATA__(data->__id, __isi)
	}
		// write !!
	__nname__ __curr = (__nname__) (data->__dop).__op2;
	__nname__ __ctmp;
	if (data->__id == STR)
	{
		__ctmp = __create_n();
		__ctmp->__name = __isi;
		__ctmp->__next = NULL;
	}
	else
	{
		char *e = malloc(strlen(__isi));
		sprintf(e, "%s\0", __isi);
		__ctmp = __create_n();
		__ctmp->__name = e;
		// printf("\na\t %d", __ctmp -> __name[strlen(__ctmp -> __name) -1]);
		__ctmp->__next = NULL;
	}
	__curr = __insert_n(__curr, __ctmp);
	if (!((data->__dop).__op2))
		(data->__dop).__op2 = __curr;
}

void naddarr_iter(NDATA * data, const char *path, const char *elem, short type, short encrypt_flags)
{
	if (!data)
		return;
	__DEFINE_VARS__ 
	if (*_lock != -1)
		  return;
	__nname__ __path = (path) ? __con_pathNode__(path) : NULL;
	if (__check_and_pointE__(data, __path, elem))
	{
		fseek(_open, *_sigPos, 0);
		*_errnum = EEEx;
		return;
	}
	register int _y = 0;
	strcpy(_temp, elem);
#ifdef _ENC_
	__edStr__(_temp, ENC);
#endif
	__mem_rel__(data);
	//char *__ftmp = __mem_alc__(data, STR, FALSE, strlen(_filepath) + 4);
	char *__ftmp = malloc(strlen(_filepath)+4);
	sprintf(__ftmp, "%s.tmp", _filepath);
	FILE *_tmp = fopen(__ftmp, "w+");
	off_t _pos = ftell(_open);
	off_t _p1 = 0;
	fseek(_open, 0, 0);
	for (; _p1 < _pos; _p1++)
		putc(getc(_open), _tmp);
	putc(_ARR_ID_, _tmp);
	putc(type, _tmp);
	for (_p1 = 0; (_y = _temp[_p1]) != '\0'; _p1++)
		putc(_y, _tmp);
	putc(_LEN_, _tmp);
	data->__id = type;
	(data->__fop).__op2 = _tmp;
	short *_ef = (short *) malloc(sizeof(short));
	*_ef = encrypt_flags;
	(data->__dop).__op1 = _ef;
	data->__lock = -5;
	free(__ftmp);
	data->__errnum = NE;
}

void nnext_addarr(NDATA * data, void *content)
{
	if (!data)
		return;
	__DEFINE_VARS__ if (*_lock != -5)
		  return;
	register int x, y;
	char *__isi;
	__nname__ __curr = (__nname__) (data->__dop).__op2;
	__nname__ __ctmp;
	short *_ef = (data->__dop).__op1;
	switch (data->__id)
	{
	case INT:
		{
			int *result = content;
			sprintf(_temp, "%d", *result);
			__isi = _temp;
		}
		break;
	case DOUBLE:
		{
			double *result = content;
			sprintf(_temp, "%lf", *result);
			__isi = _temp;
		}
		break;
	case BOOL:
		{
			short *result = content;
			sprintf(_temp, "%d", *result);
			__isi = _temp;
		}
		break;
	case LONG:
		{
			long long int *result = content;
			sprintf(_temp, "%lld", *result);
			__isi = _temp;
		}
		break;
	case STR:
		{
			char *str = content;
			__ctmp = __create_n();
			__ctmp->__name = (char *)malloc(strlen(str));
			strcpy(__ctmp->__name, str);
			if(*_ef)
				__edStr__(__ctmp->__name, ENC);
			__ctmp->__next = NULL;
			__curr = __insert_n(__curr, __ctmp);
			if (!((data->__dop).__op2))
				(data->__dop).__op2 = __curr;
			*_errnum = NE;
			return;
		}
	case CHR:
		{
			char *result = content;
			if(*_ef)
				_temp[0] = __encStr__(*result);
			else
				_temp[0] = *result;
			_temp[1] = '\0';
			__isi = _temp;
		}
	}
	if(*_ef)
		__ENCRYPT_DATA__(data->__id, __isi)
		// write !!
	char *e = malloc(strlen(__isi));
	sprintf(e, "%s\0", __isi);
	__ctmp = __create_n();
	__ctmp->__name = e;
	// printf("\na\t %d", __ctmp -> __name[strlen(__ctmp -> __name) -1]);
	__ctmp->__next = NULL;
	__curr = __insert_n(__curr, __ctmp);
	if (!((data->__dop).__op2))
		(data->__dop).__op2 = __curr;
}

 // for saving the iteration
void nsave(NDATA * data)
{
	if (!data)
		return;
	if (data->__lock == -1 || data->__lock == -2 || !(data->__lock))
		return;
	__DEFINE_VARS__
	register int x, y;
	switch (*_lock)
	{
	case -3:
		{
			while ((x = getc(_open)) != -1)
				putc(x, (data->__fop).__op2);
			fclose((FILE *) (data->__fop).__op2);
			fclose(_open);
			rename(data->__temp, _filepath);
			_open = fopen(_filepath, "r+");
			(data->__fop).__op2 = NULL;
		}
		break;
	case -4:
		{
			y = 0;
			while ((x = getc(_open)) != _LEN_ && x != _C_BUKA_)
				_temp[y++] = x;
			_temp[y] = '\0';
			int length = atoi(_temp);
			int length1 = __get_length((__nname__) (data->__dop).__op2);
			length += length1;
			// pass size arr
			sprintf(_temp, "%d", length);
			for (x = 0; _temp[x] != '\0'; x++)
				putc(_temp[x], (data->__fop).__op2);
			if ((data->__id) == STR)
			{
				putc(_LEN_, (FILE *) (data->__fop).__op2);
				while ((x = getc(_open)) != _C_BUKA_)
					putc(x, (FILE *) (data->__fop).__op2);
				__nname__ ct = (__nname__) (data->__dop).__op2;
				while (ct)
				{
					putc(_LEN_SEP_, (FILE *) (data->__fop).__op2);
					sprintf(_temp, "%d", strlen(ct->__name));
					for (x = 0; (y = _temp[x]) != '\0'; x++)
						putc(y, (FILE *) (data->__fop).__op2);
					ct = ct->__next;
				}
			}
			putc(_C_BUKA_, (FILE *) (data->__fop).__op2);
			while ((x = getc(_open)) != _C_TUTUP_)
				putc(x, (FILE *) (data->__fop).__op2);
			// passing content
			__nname__ _ct = (__nname__) (data->__dop).__op2;
			while (_ct)
			{
				putc(_ARR_SEP_, (FILE *) (data->__fop).__op2);
				for (x = 0; (y = (_ct->__name)[x]) != '\0'; x++)
					putc(y, (FILE *) (data->__fop).__op2);
				_ct = _ct->__next;
			}
			(data->__dop).__op2 = __del_n((__nname__) (data->__dop).__op2);
			putc(_C_TUTUP_, (FILE *) (data->__fop).__op2);
			while ((x = getc(_open)) != -1)
				putc(x, (FILE *) (data->__fop).__op2);
			data->__idptr = 0;
			fclose((FILE *) (data->__fop).__op1);
			fclose((FILE *) (data->__fop).__op2);
			sprintf(_temp, "%s.tmp", _filepath);
			rename(_temp, _filepath);
			free((data->__dop).__op1);
			(data->__dop).__op1 = NULL;
			(data->__fop).__op1 = _open = fopen(_filepath, "r+");
		}
		break;
	case -5:
		{
			y = 0;
			int length = __get_length((__nname__) (data->__dop).__op2);
			// pass size arr
			sprintf(_temp, "%d", length);
			for (x = 0; _temp[x] != '\0'; x++)
				putc(_temp[x], (FILE *) (data->__fop).__op2);
			if ((data->__id) == STR)
			{
				putc(_LEN_, (FILE *) (data->__fop).__op2);
				__nname__ ct = (__nname__) (data->__dop).__op2;
				while (ct)
				{
					sprintf(_temp, "%d", strlen(ct->__name));
					for (x = 0; (y = _temp[x]) != '\0'; x++)
						putc(y, (FILE *) (data->__fop).__op2);
					if ((ct = ct->__next) != NULL)
						putc(_LEN_SEP_, (FILE *) (data->__fop).__op2);
				}
			}
			putc(_C_BUKA_, (FILE *) (data->__fop).__op2);
			// passing content
			__nname__ _ct = (__nname__) (data->__dop).__op2;
			while (_ct)
			{
				for (x = 0; (y = (_ct->__name)[x]) != '\0'; x++)
					putc(y, (FILE *) (data->__fop).__op2);
				if ((_ct = _ct->__next) != NULL)
					putc(_ARR_SEP_, (FILE *) (data->__fop).__op2);
			}
			(data->__dop).__op2 = __del_n((__nname__) (data->__dop).__op2);
			putc(_C_TUTUP_, (FILE *) (data->__fop).__op2);
			while ((x = getc(_open)) != -1)
				putc(x, (FILE *) (data->__fop).__op2);
			data->__idptr = 0;
			fclose(_open);
			fclose((FILE *) (data->__fop).__op2);
			sprintf(_temp, "%s.tmp", _filepath);
			rename(_temp, _filepath);
			free((data->__dop).__op1);
			(data->__dop).__op1 = NULL;
			(data->__fop).__op1 = _open = fopen(_filepath, "r+");
		}
		break;
	case -6:
		{
			string *d = (string *) (data->__dop).__op2;
			short _en_flags = getc(_open) - '0';
			int len;
			while (getc(_open) != _LEN_);
			off_t p1 = ftell(_open);
			y = 0;
			while ((x = getc(_open)) != _C_BUKA_)
				_temp[y++] = x;
			_temp[y] = '\0';
			len = atoi(_temp);
			char *_ctmp = malloc(slen(d) + len);
			y = 0;
			while ((x = getc(_open)) != _C_TUTUP_)
				_ctmp[y++] = x;
			off_t p2 = ftell(_open);
			_ctmp[y] = '\0';
			if (_en_flags)
				__edStr__(_ctmp, DEC);
			char *sa = _ctmp + (y);
			sget(d, sa, 0, slen(d) - 1);

			(data->__dop).__op2 = NULL;
			len += slen(d);
			// printf("val = %s, length = %d\n\n\n\n\n", _ctmp, len);
			// /////////////// open new file

			if (_en_flags)
				__edStr__(_ctmp, ENC);
			fseek(_open, 0, 0);
			off_t xz = 0;
			char *vmt = _temp + (strlen(_filepath) + 5);
			sprintf(_temp, "%s.tmp", _filepath);
			FILE *_op = fopen(_temp, "w+");
			for (; xz < p1; xz++)
				putc(getc(_open), _op);
			sprintf(vmt, "%d", len);
			for (xz = 0; vmt[xz] != '\0'; xz++)
				putc(vmt[xz], _op);
			putc(_C_BUKA_, _op);
			for (xz = 0; _ctmp[xz] != '\0'; xz++)
				putc(_ctmp[xz], _op);
			fseek(_open, p2 - 1, 0);
			while ((x = getc(_open)) != -1)
				putc(x, _op);
			fclose(_op);
			fclose(_open);
			(data->__dop).__op2 = NULL;
			rename(_temp, _filepath);
			(data->__fop).__op1 = _open = fopen(_filepath, "r+");
			free(_ctmp);
			sfree(d);
		}
	}
	*_lock = -1;
	fseek(_open, *_sigPos, 0);
}
