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
 */


/* DO NOT EDIT THIS FILE - it is machine generated */
#include "NodeData.h"
#include <android/log.h>
#include "com_mylexz_utils_NodeData.h"
#ifdef _Included_com_mylexz_utils_NodeData
typedef struct __lstNDATA{
	NDATA *data;
	int __desc;
	struct __lstNDATA *ptr;
}lNDATA;
jint __getNDesc(JNIEnv *, jobject);
void __setNDesc(JNIEnv *, jobject, jint);
lNDATA *__insertlN(lNDATA *, lNDATA *);
lNDATA *__relNode(lNDATA *, int);
lNDATA *__creatNode();
NDATA *__lNcont(lNDATA *, int); 
int __genDesc(lNDATA *);
jobject _htmp;
jobject getOSTR(JNIEnv *env, jobject thiz, const char *s){
	jclass cls = (*env)->GetObjectClass(env, thiz);
	jfieldID fid = (*env)->GetFieldID(env, cls, s, "Ljava/lang/String;");
	return (*env)->GetObjectField(env, thiz, fid);
}
lNDATA *__lntemp, *__lncurr = NULL;
JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_open(JNIEnv *env, jobject thiz){
	jint __desc = __getNDesc(env, thiz);
	if(__desc != -1)return;
	__lntemp = __creatNode();
	__desc = __genDesc(__lncurr);
	__lntemp -> __desc = __desc;
	__lntemp -> ptr = NULL;
	jstring filename = getOSTR(env, thiz, "__filepath");
	jstring signature = getOSTR(env, thiz, "__signature");
	const char *fnm = (*env)->GetStringUTFChars(env, filename, 0);
	const char *snm = (*env)->GetStringUTFChars(env, signature, 0);
	__lntemp -> data = nopen(fnm, snm);
	(*env)->ReleaseStringUTFChars(env, filename, fnm);
	(*env)->ReleaseStringUTFChars(env, signature, snm);
	int err = nget_error(__lntemp -> data);
	if(err != 0) {
		__lntemp = __relNode(__lntemp, __desc);
		return;
	}
	__lncurr = __insertlN(__lncurr, __lntemp);
	__setNDesc(env, thiz, __desc);
	return;
}

JNIEXPORT jint JNICALL Java_com_mylexz_utils_NodeData_close(JNIEnv *env, jobject thiz){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return -1;
	__lncurr = __relNode(__lncurr, __desc);
	__setNDesc(env, thiz, -1);
	return 0;
}

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_addNode(JNIEnv *env, jobject thiz, jstring path, jstring node){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *elm;
	const char *dt;
	if(node == NULL)return;
	if(path == NULL) elm = NULL;
	else elm = (*env)->GetStringUTFChars(env, path, 0);
	dt = (*env)->GetStringUTFChars(env, node, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	nadd_node(d, elm, dt);
	if(elm) (*env)->ReleaseStringUTFChars(env, path, elm);
	(*env)->ReleaseStringUTFChars(env, node, dt);
}

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_delNode(JNIEnv *env, jobject thiz, jstring fullpath){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *fp;
	if(fullpath == NULL)return;
	fp = (*env)->GetStringUTFChars(env, fullpath, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	ndel_node(d, fp);
	(*env)->ReleaseStringUTFChars(env, fullpath, fp);
}

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_clearContents(JNIEnv *env, jobject thiz, jstring fullpath){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *fp;
	if(fullpath == NULL)return;
	fp = (*env)->GetStringUTFChars(env, fullpath, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	if(!d){
		(*env)->ReleaseStringUTFChars(env, fullpath, fp);
		return;
	}
	nclear_body(d, fp);
	(*env)->ReleaseStringUTFChars(env, fullpath, fp);
}

JNIEXPORT jobjectArray JNICALL Java_com_mylexz_utils_NodeData_listContents(JNIEnv *env, jobject thiz, jstring fullpath){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return NULL;
	const char *fp;
	jobjectArray p;
	if(fullpath == NULL)fp = NULL;
	else fp = (*env)->GetStringUTFChars(env, fullpath, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	char **contents = nlist_body(d, fp);
	if(!contents){
		if(fp) (*env)->ReleaseStringUTFChars(env, fullpath, fp);
		return NULL;
	}
	jsize size = 0;
	jint count;
	for(count = 0; contents[count] != NULL; count++)size++;
	p = (*env) -> NewObjectArray(env, size, (*env) -> FindClass(env, "java/lang/String"), NULL);
	for(count = 0; count < size; count++)
		(*env) -> SetObjectArrayElement(env, p, count, (*env) -> NewStringUTF(env, contents[count]));
	if(fp) (*env)->ReleaseStringUTFChars(env, fullpath, fp);
	return p;
}

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_addIntData(JNIEnv *env, jobject thiz, jstring path, jstring elem, jint data, jboolean encrypt_flags){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *elm;
	const char *dt;
	if(elem == NULL)return;
	if(path == NULL) dt = NULL;
	else dt = (*env)->GetStringUTFChars(env, path, 0);
	elm = (*env)->GetStringUTFChars(env, elem, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	int val = (int) data;
	nadd_data(d, dt, elm, INT, &val, ((encrypt_flags)?1:0));
	if(dt) (*env)->ReleaseStringUTFChars(env, path, dt);
	(*env)->ReleaseStringUTFChars(env, elem, elm);
 }

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_addLongData(JNIEnv *env, jobject thiz, jstring path, jstring elem, jlong data, jboolean encrypt_flags){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *elm;
	const char *dt;
	if(elem == NULL)return;
	if(path == NULL) dt = NULL;
	else dt = (*env)->GetStringUTFChars(env, path, 0);
	elm = (*env)->GetStringUTFChars(env, elem, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	nadd_data(d, dt, elm, LONG, &data, ((encrypt_flags)?1:0));
	if(dt) (*env)->ReleaseStringUTFChars(env, path, dt);
	(*env)->ReleaseStringUTFChars(env, elem, elm);
}

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_addCharData(JNIEnv *env, jobject thiz, jstring path, jstring elem, jchar data, jboolean encrypt_flags){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *elm;
	const char *dt;
	if(elem == NULL)return;
	if(path == NULL) dt = NULL;
	else dt = (*env)->GetStringUTFChars(env, path, 0);
	elm = (*env)->GetStringUTFChars(env, elem, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	char val = (char) data;
	nadd_data(d, dt, elm, CHR, &val, ((encrypt_flags)?1:0));
	if(dt) (*env)->ReleaseStringUTFChars(env, path, dt);
	(*env)->ReleaseStringUTFChars(env, elem, elm);
}

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_addBoolData(JNIEnv *env, jobject thiz, jstring path, jstring elem, jboolean data, jboolean encrypt_flags){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *elm;
	const char *dt;
	if(elem == NULL)return;
	if(path == NULL) dt = NULL;
	else dt = (*env)->GetStringUTFChars(env, path, 0);
	elm = (*env)->GetStringUTFChars(env, elem, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	short val =  (data)?1:0;
	nadd_data(d, dt, elm, BOOL, &val, ((encrypt_flags)?1:0));
	if(dt) (*env)->ReleaseStringUTFChars(env, path, dt);
	(*env)->ReleaseStringUTFChars(env, elem, elm);
}

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_addStrData(JNIEnv *env, jobject thiz, jstring path, jstring elem, jstring data, jboolean encrypt_flags){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *elm;
	const char *dt;
	if(elem == NULL)return;
	if(path == NULL) dt = NULL;
	else dt = (*env)->GetStringUTFChars(env, path, 0);
	elm = (*env)->GetStringUTFChars(env, elem, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	const char *cstr = (data!=NULL)?(*env) -> GetStringUTFChars(env, data, 0):NULL;
	nadd_data(d, dt, elm, STR, cstr, ((encrypt_flags)?1:0));
	if(dt) (*env)->ReleaseStringUTFChars(env, path, dt);
	(*env)->ReleaseStringUTFChars(env, elem, elm);
	if(cstr) (*env)->ReleaseStringUTFChars(env, data, cstr);
}

JNIEXPORT void JNICALL Java_com_mylexz_utils_NodeData_addDoubleData(JNIEnv *env, jobject thiz, jstring path, jstring elem, jdouble data, jboolean encrypt_flags){
	jint __desc = __getNDesc(env, thiz);
	if(__desc == -1)return;
	const char *elm;
	const char *dt;
	if(elem == NULL)return;
	if(path == NULL) dt = NULL;
	else dt = (*env)->GetStringUTFChars(env, path, 0);
	elm = (*env)->GetStringUTFChars(env, elem, 0);
	NDATA *d = __lNcont(__lncurr, __desc);
	nadd_data(d, dt, elm, DOUBLE, &data, ((encrypt_flags)?1:0));
	if(dt) (*env)->ReleaseStringUTFChars(env, path, dt);
	(*env)->ReleaseStringUTFChars(env, elem, elm);
}



/******/
  
jint __getNDesc(JNIEnv *env, jobject thiz){
	jclass cls = (*env)->GetObjectClass(env, thiz);
	jfieldID fid = (*env)->GetFieldID(env, cls, "__id", "I");
	return (*env)->GetIntField(env, thiz, fid);
	 
}
void __setNDesc(JNIEnv *env, jobject thiz, jint val){
	jclass cls = (*env)->GetObjectClass(env, thiz);
	jfieldID fid = (*env)->GetFieldID(env, cls, "__id", "I");
	(*env)->SetIntField(env, thiz, fid, val);
	 
}
lNDATA *__insertlN(lNDATA *curr, lNDATA *tmp){
	if(!curr)return tmp;
	curr -> ptr = __insertlN(tmp, curr -> ptr);
}
lNDATA *__relNode(lNDATA *curr, int __desc){
	lNDATA *tmp;
	if(!curr) return curr;
	if(curr -> __desc == __desc)
	{
		nclose((curr->data));
		tmp = curr;
		curr = curr->ptr;
		free(tmp);
		return(curr);
	}
	curr -> ptr = __relNode(curr -> ptr, __desc);
	return(curr);
}
lNDATA *__creatNode(){
	return (lNDATA *) malloc(sizeof(lNDATA));
}
NDATA *__lNcont(lNDATA *curr, int __desc){
	if(__desc == -1)return NULL;
	NDATA *addr = NULL;
	while(curr){
		if(curr -> __desc == __desc){
			addr = (curr -> data);
			break;
		}
		curr = curr -> ptr;
	}
	return addr;
}
int __genDesc(lNDATA *curr){
	if(curr == NULL)return 0;
	int __desc;
	while(curr){
		__desc = curr -> __desc;
		curr = curr -> ptr;
	}
	return ++__desc;
}
#endif
