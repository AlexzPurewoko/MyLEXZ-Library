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
 *	@(#)PrimitiveData.c	v5 @@ SOURCE FILE @@
 * Last updated in : Friday, Dec, 1, 2017 in 04:57 PM
 * Time Zone : Jakarta, GMT+7 , WIB */
  /**
 * PrimitiveData is a library used to store and lead to the data
 * also operations in supporting a wide range of applications in order to use
 * and operate as well as managing data quickly, accurate,
 * and keep the privacy not leaked to other users.
 * These data in every element contains a cache memory for 32kB,
 * due to the operation input and output data from/ to in the file.
 * In each element data input must be less than a long 32768 character,
 * due to avoid the operation error in the input or output.
 * So, you have to the manner in its use in order to as expected also smoothly.
 * Take it easy, the data in the file will be compiled or translated or encrypted in order to awake privacy.
 */
  /*
  * Changelog :
  * Bugs fixed
  * the long variable type parameters now is changed to long long int to supporting jlong
  * readStringPoint now is support multi process for both readStringPoint and another processes neither readStringPoint
  * 
  */
  // <!--- DO NOT EDIT THIS SOURCE FILE OR YOU HAVE AN ERROR OCCURED IF YOU NOT AUTHOR THIS FILE!!! -->
  #include "PrimitiveData.h"
/******** ENCRYPTOR AND DECRYPTOR ********/
// Encryptor for number
int encNum(int x){
	return( 2 << 3 )+ ( x - (( (1 << 5 )+ (1 << 3)+ (1 << 1)) +  ~(~0 + ~0)));
}
// Decryptor for number
int decNum(int x){
	return (2 << 3) + (x + ((1<<3)+(1<<1)) + ~(~0 + ~0));
}
// Encryptor for String
int encStr(int x){
	return x + ((1<<7)-(1<<2));
}
// Decryptor for String
int decStr(int x){
	return x - ((1<<7)-(1<<2));
}
/**
 * edNum( String_of_number_to_enc_or_dec, mode_dec_or_enc);
 * Encrypt or Decrypt the String of number into its string again
 * without return
 */
void edNum(char num[], int mode){
	reverse(num);
	size_t len = strlen(num), x = 0;
	register int chr;
	for (; x < len; x++){
		if(mode == ENC)
		    chr = encNum(num[x]);
		else if(mode == DEC)
		    chr = decNum(num[x]);
		num[x] = chr;
	}
}
/**
 * edStr( String_to_enc_or_dec, mode_dec_or_enc);
 * Encrypt or Decrypt the String into its string again
 * without return
 */
void edStr(char str[], int mode){
	reverse(str);
	size_t len = strlen(str), x = 0;
	register int chr;
	for (; x < len; x++){
		if( mode == ENC)
		    chr = encStr(str[x]);
		else if(mode == DEC)
		    chr = decStr(str[x]);
		str[x] = chr;
	}
}
/**
 * this function is a part of checkExistsElem(FILE*, char [], off_t)
 * so we don't have the way of function used
 */
int getStr(char temp[], FILE* read){
	char tmp;
	int x = 0;
	while((tmp = getc(read))!= BUKA)
		temp[x++] = tmp;
	temp[x] = '\0';
	return x;
}
/**
 * Cheking signature of the file
 * signature are placed at the top of file
 * so this function can protect the security of file
 * and can define its own file
 * usage :
 * checkSignature( your_file, string_signature)
 * return 0 if operation isn't success
 * and return the offset or cursor position after signature
 * in a file if success
 */
int checkSignature(char temp[], FILE* f, const char *signature){

	if(f == NULL)return 0;
	int elem, chr;
	for(elem = 0; (chr = getc(f)) != '\n' && chr != -1; elem++)temp[elem] = chr;
	//printf("%s", temp);
	if(chr == -1)return 0;
	temp[elem] = '\0';
	//printf("%d",cmp(temp,signature));
	edStr(temp, DEC);
	int bol = cmp(temp, signature);
	if(bol)
		return ++elem;
	else
		return 0;

}
/**
 * This function is used to check whether
 * elem is found or not
 * if elem is found, it will locate the position of reading file into
 * the beginning of the elements declarations
 * and return 1
 * if elem isn't found return 0
 * how to use :
 * checkExistsElem(pFile, "elements", 6);
 * this func is a part of any function in this program
 * so i don't have a detail about this
 */
int checkExistsElem(char temp[], FILE* read, const char *elem, off_t offset){
	fseek(read, offset, 0);
	int tmp;
	int x = 0;
	while((tmp = getc(read))!=-1){
		if(tmp == ID || tmp == ID_ARR){
			tmp = getc(read);
			getStr(temp, read);
			if(cmp(temp, elem)){
				fseek(read, ftell(read)-strlen(elem)-3, 0);
				return 1;
			}
		}
	}
	return 0;
}
void getData(char temp[], FILE* f, int pos){
	register int chr;
	int x = 0, y =0;
	while(1){
		chr = getc(f);
		if (x == pos)
		    if(chr == TUTUP || chr == ARR_SEPARATOR){
		    	temp[y] = '\0';
		    	return;
		    }
		    else temp[y++] = chr;
		else
		    if(chr == ARR_SEPARATOR)x++;
		    else if(chr == TUTUP)return;
	}
	return;
}
size_t count(FILE* f, off_t offset){
	size_t x, y = 0;
	while((x = getc(f))!=TUTUP)if(x == ARR_SEPARATOR)y++;
	fseek(f, offset, 0);
	return ++y;
}
void f(char temp[], const char *s){
	int x, y;
	for (x = y = 0; s[x] != '\0'; x++,y++)
	    if(s[x] != ' ' && s[x] != '\n' && s[x] != '\t')
	        temp[y] = s[x];
	   temp[y] = '\0';
    //strcpy(s, temp);
}
void __passToFile(char temp[], FILE* f, int x, int len){
	fprintf(f, "%s", temp);
	if(x != len-1)
		putc(ARR_SEPARATOR, f);
	else
		putc(TUTUP, f);
}

/*
 * Write a new PrimitiveData Format file using signature
 * must using signature!
 * usage :
 * writeNew(filename, signature);
 */
int writeNew(char temp[], const char *file, const char *sig){
	FILE* fj = fopen(file, "w");
	f(temp, sig);
	edStr(temp, ENC);
	fprintf(fj, "%s\n", temp);
	fclose(fj);
	return strlen(temp);
}

/* This function is part of getElem(); */
void __putData(char temp[], FILE* read){
	size_t x = 0;
	register int chr;
	while((chr = getc(read))!=BUKA);
	while((chr = getc(read))!=TUTUP)temp[x++] = chr;
	temp[x] = '\0';
	//fclose(read);
}
/*
 * This method (__pointToElem()) is a part of setDataAtPosArr()
 * so, you cannot implement this module
 * or you have an error occurs
 */
void __pointToElem(FILE* f, off_t* pos1, off_t* pos2, size_t pos){
	size_t x = 0;
	register int c, lock = 0;
	while(1)
		if ( x == pos){
			c = getc(f);
			//printf("%c\n", c);

			if(lock){
		    	if (c ==TUTUP || (c ==ARR_SEPARATOR)){
		    		*pos2 = ftell(f)-1;
		    		return;
		    		}
			}
		    else {
		    	lock = 1;
		    	*pos1 = ftell(f)-1;
		        }


		}
		else {
			c = getc(f);
			if(c == ARR_SEPARATOR) x++;
			else if(c == TUTUP)return;
		}

	return;
}

/******* BASIC FUNCTION OF PROGRAM ******/
/**
 * used to open a file and placing into
 * temporary static file
 * open(filename, signature);
 */
PDATA* openf(const char *file, const char *sig){
	//Checking files.....
//	if(rd != NULL)return;
	//make new PDATA with malloc
	PDATA* data = malloc(sizeof(PDATA));
	if(checkExists(file))writeNew(data->temp, file, sig);

	//Check Signature
	data->rd = fopen(file, "r+");
	int offset = checkSignature(data->temp, data->rd, sig);
	if(!offset){
		fclose(data->rd);
		data->rd = NULL;
	    data->errnum = -1;
		return data;
	}
	strcpy(data->tempFile, file);
	fseek(data->rd, offset, 0);
	data->errnum = 0;
	data->lockPointer = -1;
	data->offt = 0;
	data->sigOffset = offset;
	return data;
}
/**
 * used to close a running file
 */
 void closef(PDATA* data){
	if((data->rd) != NULL){
		data->errnum = fclose(data->rd);
	}
	data->rd = NULL;
	// releasing allocation memory by malloc
	free(data);
	return;
}
/**
 * used to get an errors after calling a function
 */
int getErrors(PDATA* data){
	return data->errnum;
}
/**
 * addElem(element, tipeData<int>, data);
 * used to add element with specified data and tipeData to
 * declare a data.
 */
void addElem(PDATA* pdata, const char *elem, int tpData, void* data){
	if(pdata == NULL)return;
	int offset = ftell(pdata->rd);
	int bl = checkExistsElem(pdata->temp, pdata->rd, elem, offset);
	if(bl) {
		pdata->errnum = -2;
		fseek(pdata->rd, offset, 0);
    	return;
	}
	//Querrying
	switch(tpData){
		case INT : {
			int *t = data;
			sprintf(pdata->temp, "%d", *t);
			edNum(pdata->temp, ENC);
		}
		break;
		case STR : {
			char *s = data;
			strcpy(pdata->temp, s);
			edStr(pdata->temp, ENC);
		}
		break;
		case CHR : {
			char* s = data;
			int en = encStr(*s);
			pdata->temp[0] = en;
			pdata->temp[1] = '\0';
		}
		break;
		case LONG : {
			long long int* l = data;
			sprintf(pdata->temp, "%lld", *l);
			edNum(pdata->temp, ENC);
		}
		break;
		case DOUBLE : {
			double* d = data;
			sprintf(pdata->temp, "%lf", *d);
			edNum(pdata->temp, ENC);
		}
		break;
		default : {
			pdata->errnum = -3;
			fseek(pdata->rd, offset, 0);
			return;
		}
	}
	fprintf(pdata->rd,"%c%c%s%c%s%c", ID, tpData, elem, BUKA, pdata->temp, TUTUP);
	pdata->errnum = 0;
	fseek(pdata->rd, offset, 0);
	return;
}
/**
 * deleteElem(nameElement);
 * deleting Elements at specified nameElement
 */
void deleteElem(PDATA* pdata, const char *elem){
	if (pdata == NULL)return;
	off_t offt = ftell(pdata->rd);
	int bl = checkExistsElem(pdata->temp, pdata->rd, elem, offt);
	if(!bl) {
		pdata->errnum = -2;
		fseek(pdata->rd, offt, 0);
		return;
	}
	//create temp file mode write
	sprintf(pdata->temp, "%s.tmp", pdata->tempFile);
	//printf("\n\n%s", temp);
	FILE* nwF = fopen(pdata->temp, "w+");
	//offset from
	off_t offset = ftell(pdata->rd);
	//Getting next element
	register int chr;
	size_t x = 0;
	while(((chr = getc(pdata->rd)) != -1)&&chr != TUTUP);
	off_t offsetTo = ftell(pdata->rd);
	// Passing next element onto temp file
	while((chr = getc(pdata->rd)) != -1)putc(chr, nwF);
	// seek file into current delete elem
	fseek(pdata->rd, offset, 0);
	fclose(nwF);
	nwF = fopen(pdata->temp, "r+");
	// saving all value in temp file to current file
	while((chr = getc(nwF))!= -1)putc(chr, pdata->rd);
	fclose(nwF);
	remove(pdata->temp);
	fseek(pdata->rd, offt, 0);
	fclose(nwF);
	pdata->errnum = 0;
	return;
}

/**
 * getElem(nameElement);
 * get Data value with specified nameElement
 * data value is converted into void* because to accepting
 * all data types, because void* are just copy the address
 * memory.
 * This data isn't static, so the data can lost after
 * the end of function encountered. But i can keep the memory until
 * the next declaration. And I don't guarantee if the data is lost.
 * to avoid the data lost, you must copy the data into new variable
 * like this :
 * void* v = getElem(file, sig, "IntElem");
 * int* data = v;
 * int newData = *data; //copy to newData

 * This can save the Data at a long time used
 */
void* getElem(PDATA* pdata, const char *elem){
	if(pdata == NULL) return NULL;
	off_t offset = ftell(pdata->rd);
	int bl = checkExistsElem(pdata->temp, pdata->rd, elem, offset);
	if(!bl){
		pdata->errnum = -2;
		fseek(pdata->rd, offset, 0);
		return NULL;
	}
	int qID = getc(pdata->rd);
	int qTPDT = getc(pdata->rd);

	if(qID == ID_ARR){
		pdata->errnum = -3;
		fseek(pdata->rd, offset, 0);
		return NULL;
	}
	switch (qTPDT){
		case INT:
		{
			__putData(pdata->temp, pdata->rd);
			edNum(pdata->temp, DEC);
			fseek(pdata->rd, offset, 0);
			int res = atoi(pdata->temp);
			void* ptr = &res;
			pdata->errnum = 0;
			return ptr;
		}
		case STR:
		{
            __putData(pdata->temp, pdata->rd);
			edStr(pdata->temp, DEC);
			fseek(pdata->rd, offset, 0);
			void* ptr = pdata->temp;
			pdata->errnum = 0;
			return ptr;
		}
		case CHR :
		{
			__putData(pdata->temp, pdata->rd);
			char t = decStr(pdata->temp[0]);
			fseek(pdata->rd, offset, 0);
			void* v = &t;
			pdata->errnum = 0;
			return v;
		}
		case DOUBLE : {
			__putData(pdata->temp, pdata->rd);
			edNum(pdata->temp, DEC);
			fseek(pdata->rd, offset, 0);
			double resd = atof(pdata->temp);
			void* ptr = &resd;
			pdata->errnum = 0;
			return ptr;
		}
		case LONG : {
			__putData(pdata->temp, pdata->rd);
			edNum(pdata->temp, DEC);
			fseek(pdata->rd, offset, 0);
			long long int resl = atoll(pdata->temp);
			void* ptr = &resl;
			pdata->errnum = 0;
			return ptr;
		}
		default:{
			pdata->errnum = -4;
			fseek(pdata->rd, offset, 0);
		    return NULL;
		}
	}
	return NULL;
}
/**
 * setElem(nameElements, content_data)
 * Setting data element at nameElements
 * By changing old data at elements to new data value
 */
void setElem(PDATA* pdata, const char *elem, void* reserved){
	if(pdata == NULL)return;
	off_t offset = ftell(pdata->rd);
	int bl = checkExistsElem(pdata->temp, pdata->rd, elem, offset);
	if(!bl){
	    pdata->errnum = -2;
		fseek(pdata->rd, offset, 0);
		return;
	}
	int tc,c;
	tc = getc(pdata->rd);
	if (tc == ID_ARR){
		fseek(pdata->rd, offset, 0);
		pdata->errnum = -3;
		return;
	}
	tc = getc(pdata->rd);
	while ((c = getc(pdata->rd))!=BUKA);
	off_t pos1 = ftell(pdata->rd);
	while ((c = getc(pdata->rd))!=TUTUP);
	off_t pos2 = ftell(pdata->rd);
	char n[32];
	switch(tc){
		case INT:
		{
		    int* dat = reserved;
		    sprintf(pdata->temp, "%d", *dat);
			edNum(pdata->temp, ENC);
		}
		break;
		case STR:
		{
		    sprintf(pdata->temp,"%s", reserved);
			edStr(pdata->temp, ENC);
		}
		break;
		case LONG:
		{
		    long long int* dat = reserved;
		    sprintf(pdata->temp, "%lld", *dat);
			edNum(pdata->temp, ENC);
		}
		break;
		case DOUBLE:
		{
		    double* dat = reserved;
		    sprintf(pdata->temp, "%lf", *dat);
			edNum(pdata->temp, ENC);
		}
		break;
		case CHR:
		{
		    char* s = reserved;
		    sprintf(pdata->temp, "%c", encStr(*s));

		}
		break;
		default:
		    fseek(pdata->rd, offset, 0);
		    pdata->errnum = -4;
			return;
	}
	fseek(pdata->rd, 0, 0);
	sprintf(n, "%s.tmp", pdata->tempFile);
	FILE* nwF = fopen(n, "w+");
	register int chr, x;
	for( x = 0;(x< pos1 )&& ((chr = getc(pdata->rd))!=-1); x++)putc(chr, nwF);
	for( x = 0; pdata->temp[x] != '\0'; x++) putc( pdata->temp[x], nwF);
	fseek(pdata->rd, pos2-1, 0);
	while((chr = getc(pdata->rd))!=-1)putc(chr, nwF);
	fclose(nwF);
	fclose(pdata->rd);
	rename(n, pdata->tempFile);
	pdata->rd = fopen(pdata->tempFile, "r+");
	fseek(pdata->rd, offset, 0);
	pdata->errnum = 0;
	return;
}

/**
 * addArr(nameElement, TipeData, content_data, length_data);
 * Add Array Element in file
 * This method isn't compatible if tpData == STR
 * if you would like to use arrayStrings
 * you must call addStrArr() function
 */
void addArr(PDATA* pdata, const char *elem, int tpData, void* data, size_t dataS){
	if(pdata == NULL)return;
	off_t offset = ftell(pdata->rd);
	int bl = checkExistsElem(pdata->temp, pdata->rd, elem, offset);
	if(bl){
		fseek(pdata->rd, offset, 0);
		pdata->errnum = -2;
		return;
	}
	if (tpData == STR){
		fseek(pdata->rd, offset, 0);
		pdata->errnum = -3;
		return;
	}
	char tv[35];
	fprintf(pdata->rd,"%c%c%s%c", ID_ARR, tpData, elem, BUKA);
    /* Passing masing masing value ke temp lalu
	 * langsung simpan value temp ke file
	*/
	/***
	 * declare variable to count the loop val
	 * declare at outside loop, because for supporting in C89 mode
	 */
	 size_t count;
	for(count = 0; count < dataS; count++){
		// Convert all data in array according to the types to string formats
		// Selecting array data types with switch
		switch(tpData){
			// data INT
			case INT:{
				// first, convert void* into int*
				// because the void* not support direct dereferencing
				// operation
				int* tmp = data;
				// Two, convert array of int into string according
				// the its position
				sprintf(pdata->temp, "%d", tmp[count]);
				edNum(pdata->temp, ENC);
			}
			break;
			// data LONG
			case LONG:{
				// first, convert void* into long*
				// because the void* not support direct dereferencing
				// operation
				long long int* tmp = data;
				// Two, convert array of long into string according
				// the its position
				sprintf(pdata->temp, "%lld", tmp[count]);
				edNum(pdata->temp, ENC);
			}
			break;
			// data DOUBLE
			case DOUBLE:{
				// first, convert void* into double*
				// because the void* not support direct dereferencing
				// operation
				double* tmp = data;
				// Two, convert array of double into string according
				// the its position
				sprintf(pdata->temp, "%lf", tmp[count]);
				edNum(pdata->temp, ENC);
			}
			break;
			// data CHAR
			case CHR:{
				// first, convert void* into char*
				// because the void* not support direct dereferencing
				// operation
				char* tmp = data;
				// Two, convert array of char into string according
				// the its position
				sprintf(pdata->temp, "%c", encStr(tmp[count]));
			}
			break;
			//option default because tpData isn't compatible
			//with this program version or tpData
			//is not valid
			default : ;
		}
		// three, saving values by update into a file
		// this method is using passToFile(FILE*, int, int) function
		__passToFile(pdata->temp, pdata->rd, count, dataS);
	}
	//close the file and return 0
	fseek(pdata->rd, offset, 0);
	pdata->errnum = 0;
	return ;// indicates operation success
}
/**
 * getAllContentInArr(elements);
 * This method is used to get All Content in Array
 * or get array of an element.
 * Not for STR ARRAY!!
 */
void* getAllContentInArr(PDATA* pdata, const char *elem){
	if(pdata == NULL)return NULL;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)){
		fseek(rd, offset, 0);
		*errnum = -2;
		return NULL;
	}
	// get Identifier
	int ID = getc(rd);
	// get TipeData
	int TP = getc(rd);
	if(ID != ID_ARR || TP == STR){
		fseek(rd, offset, 0);
		*errnum = -3;
		return NULL;
	}
	int c, y  = 0;
	// Pointing the offset to BUKA char
	while ((c = getc(rd))!=BUKA);
	// save offset
	off_t pos1 = ftell(rd);
	void* x;
	switch(TP){
		case INT:
		{
			size_t cnt = count(rd, pos1);
			int tmp[cnt];
			int y = 0, z =0, z2 = 0;
			while(1){
				y = getc(rd);
				if (y == ARR_SEPARATOR){
					temp[z] = '\0';
					z = 0;
					edNum(temp, DEC);
					tmp[z2++] = atoi(temp);

				}
				else if(y == TUTUP){
					temp[z] = '\0';
					z = 0;
					edNum(temp, DEC);
					tmp[z2++] = atoi(temp);

					break;
				}
				else temp[z++] = y;
			}
			x = tmp;
			fseek(rd, offset, 0);
			*errnum = 0;
        	return x;
		}
		case LONG:
		{
			size_t cnt = count(rd, pos1);
			long long int tmp[cnt];
			int y = 0, z =0, z2 = 0;
			while(1){
				y = getc(rd);
				if (y == ARR_SEPARATOR){
					temp[z] = '\0';
					z = 0;
					edNum(temp, DEC);
					tmp[z2++] = atoll(temp);

				}
				else if(y == TUTUP){
					temp[z] = '\0';
					z = 0;
					edNum(temp, DEC);
					tmp[z2++] = atoll(temp);

					break;
				}
				else temp[z++] = y;
			}
			x = tmp;
			fseek(rd, offset, 0);
			*errnum = 0;
        	return x;
		}
		case CHR:
		{
			size_t cnt = count(rd, pos1);
			char tmp[cnt];
			register int y;
			int z = 0;
			while(1){
				y = getc(rd);
				if (y == ARR_SEPARATOR)continue;
				else if(y == TUTUP)break;
				else tmp[z++] = decStr(y);
			}
			tmp[z] = '\0';
			x = tmp;
			fseek(rd, offset, 0);
			*errnum = 0;
        	return x;
		}
		case DOUBLE :
		{
			size_t cnt = count(rd, pos1);
			double tmp[cnt];
			int y = 0, z =0, z2 = 0;
			while(1){
				y = getc(rd);
				if (y == ARR_SEPARATOR){
					temp[z] = '\0';
					z = 0;
					edNum(temp, DEC);
					tmp[z2++] = atof(temp);

				}
				else if(y == TUTUP){
					temp[z] = '\0';
					z = 0;
					edNum(temp, DEC);
					tmp[z2++] = atof(temp);

					break;
				}
				else temp[z++] = y;
			}
			x = tmp;
			fseek(rd, offset, 0);
			*errnum = 0;
        	return x;
		}
		default: x = NULL;
	}
	fseek(rd, offset, 0);
	*errnum = -4;
    return x;
}
/**
 * This method is used to get data at the position in elementArray
 * usage :
 * getContentArrAtPos(filename, signature, nameElement, position_data_in_array<int>);
 */
void* getContentArrAtPos(PDATA* pdata, const char *elem, size_t pos){
	if(pdata == NULL)  return NULL;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)){
		fseek(rd, offset, 0);
		*errnum = -2;
		return NULL;
	}
	int ID = getc(rd);
	int TP = getc(rd);
	if(ID != ID_ARR && TP != STR){
		fseek(rd, offset, 0);
		*errnum = -3;
		return NULL;
	}
	int c, y  = 0;
	while ((c = getc(rd))!=BUKA);
	off_t pos1 = ftell(rd);
	size_t sizeArr = count(rd, pos1);
	if(pos < 0 || pos >= sizeArr){
		fseek(rd, offset, 0);
		*errnum = -4;
		return NULL;
	}
	void* x;
	getData(temp, rd, pos);
	switch(TP){
		case INT:
		{
			edNum(temp, DEC);
			int __i = atoi(temp);
			x = &__i;
			fseek(rd, offset, 0);
			*errnum = 0;
			return x;
		}
		case LONG:
		{
			edNum(temp, DEC);
			long long int __l = atoll(temp);
			x = &__l;
			fseek(rd, offset, 0);
			*errnum = 0;
			return x;
		}
		case CHR:
		{
			 char __c = decStr(temp[0]);
			x = &__c;
			fseek(rd, offset, 0);
			*errnum = 0;
			return x;
		}
		case DOUBLE : {
			edNum(temp, DEC);
			 double __d = atof(temp);
			x = &__d;
			fseek(rd, offset, 0);
			*errnum = 0;
			return x;
		}
		default: x = NULL;
	}
	fseek(rd, offset, 0);
	*errnum = -5;
	return NULL;
}

/**
 * This method is used to append a content of an array at the last
 * position in the elementArray
 * usage :
 * appendArr(filename, signature, nameElement, arrayData, length_array_to_add):
 */
void appendArr(PDATA* pdata, const char *elem, void* data, size_t len){
	if(pdata == NULL)return;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	char* tempFile = pdata->tempFile;
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)) {
		fseek(rd, offset, 0);
		*errnum = -2;
		return;
	}
	char ID = getc(rd);
	char TP = getc(rd);
	if (ID != ID_ARR || TP == STR){
		fseek(rd, offset, 0);
		*errnum = -3;
		return;
	}
	register int y;
	while ((y = getc(rd))!=TUTUP);
	off_t pos1 = ftell(rd)-1;
	char n[32];
	sprintf(n, "%s.tmp", tempFile);
	fseek(rd, pos1, 0);
	FILE* rv = fopen(n, "w+");
	size_t x = 0;
	while ((y = getc(rd))!=-1)putc(y, rv);
	fseek(rd, pos1, 0);
	for(; x<len; x++){
		switch(TP){
			case INT:
			{
				int* tmp = data;
				sprintf(temp, "%d", tmp[x]);
				edNum(temp, ENC);
				break;
			}
			case LONG:
			{
				long long int* tmp = data;
				sprintf(temp, "%lld", tmp[x]);
				edNum(temp, ENC);
				break;
			}
			case CHR:{
				char* s = data;
				temp[0] = encStr(s[x]);
				temp[1] = '\0';
				break;
			}
			case DOUBLE:
			{
				double* tmp = data;
				sprintf(temp, "%lf", tmp[x]);
				edNum(temp, ENC);
				break;
			}
			default: {
				fclose(rv);
				remove(n);
				fseek(rd, offset, 0);
				*errnum = -4;
				return;
			}
		}
		//printf("\nAlloc in %d", x);
		//printf("%s\n", temp);
		fprintf(rd, "%c%s", ARR_SEPARATOR, temp);
	}
	//putc(TUTUP, rv);
	fseek(rv, 0, 0);
	while((y = getc(rv))!=-1)putc(y, rd);
	fseek(rd, offset, 0);
	fclose(rv);
	remove(n);
	*errnum = 0;
	return;
}
/**
 * This method is used to set an arrat by changing all elements
 * and take the new value of the element
 * usage :
 * setArr(nameElements, contentElements, lengthOfElement)
 */
void setArr(PDATA* pdata, const char *elem, void* data, size_t len){
	if(pdata == NULL) return;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	char* tempFile = pdata->tempFile;
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)) {
		fseek(rd, offset, 0);
    	*errnum = -2;
		return;
	}
	char ID = getc(rd);
	char TP = getc(rd);
	if (ID != ID_ARR || TP == STR){
		fseek(rd, offset, 0);
		*errnum = -3;
		return;
	}
	register int y;
	while ((y = getc(rd))!=BUKA);
	off_t pos1 = ftell(rd);
	while ((y = getc(rd))!=TUTUP);
	off_t pos2 = ftell(rd);
	char n[32];
	sprintf(n, "%s.tmp", tempFile);
	FILE* rv = fopen(n, "w");
	fseek(rd, 0,0);
	off_t t = 0;
	while(((y = getc(rd))!= -1)&&(t++ < pos1))putc(y, rv);
	size_t x = 0;
	for(; x<len; x++){
		switch(TP){
			case INT:
			{
				int* tmp = data;
				sprintf(temp, "%d", tmp[x]);
				edNum(temp, ENC);
				break;
			}
			case LONG:
			{
				long long int* tmp = data;
				sprintf(temp, "%lld", tmp[x]);
				edNum(temp, ENC);
				break;
			}
			case CHR:{
				char* s = data;
				temp[0] = encStr(s[x]);
				temp[1] = '\0';
				break;
			}
			case DOUBLE:
			{
				double* tmp = data;
				sprintf(temp, "%lf", tmp[x]);
				edNum(temp, ENC);
				break;
			}
			default: {
				fseek(rd, offset, 0);
				*errnum = -4;
				return;
			}
		}
		//printf("\nAlloc in %d", x);
		//printf("%s\n", temp);
		fprintf(rv, "%s%c",  temp, ARR_SEPARATOR);
	}
	t  = ftell(rv);
	fseek(rv, t-1, 0);
	fseek(rd, pos2-1, 0);
	while(((y = getc(rd))!= -1))putc(y, rv);
	fclose(rd);
	fclose(rv);
	rename(n, tempFile);
	rd = fopen(tempFile, "r+");
	fseek(rd, offset, 0);
	*errnum = 0;
	return;
}
/**
 * This method is used to setting data element at position in
 * an array
 * usage :
 * setDataAtPosArr(nameElements, elementType<struct data>, contentToChange, positionToChange)

 */
void setDataAtPosArr(PDATA* pdata, const char *elem, int elemType, void* data, size_t pos){
	if(pdata == NULL) return;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	char* tempFile = pdata->tempFile;
	off_t offset = ftell(rd);
	int bl = checkExistsElem(temp, rd, elem, offset);
	if(!bl) {
		fseek(rd, offset,0);
    	*errnum = -2;
		return;
	}
	char ID = getc(rd);
	char TP = getc(rd);
	if (ID != ID_ARR || TP == STR || TP != elemType){
		fseek(rd, offset, 0);
		*errnum = -3;
		return;
	}
	register int y;
	while ((y = getc(rd))!=BUKA);
	off_t pos1 = ftell(rd), pos2;
	size_t lnArr = count(rd, pos1);
	if( pos < 0 || pos >= lnArr){
		fseek(rd, offset, 0);
		*errnum = -4;
		return;
	}
//	printf("%c", getc(read));
	//printf("%d, %d", pos1, pos2);
	__pointToElem(rd, &pos1, &pos2, pos);
	char n[32];
	sprintf(n, "%s.tmp", tempFile);
	FILE* rv = fopen(n, "w");
	fseek(rd, 0, 0);
	off_t t = 0;
	while(((y = getc(rd))!= -1)&&(t++ < pos1))putc(y, rv);
		switch(TP){
			case INT:
			{
				int* tmp = data;
				sprintf(temp, "%d", *tmp);
				edNum(temp, ENC);
				break;
			}
			case LONG:
			{
				long long int* tmp = data;
				sprintf(temp, "%lld", *tmp);
				edNum(temp, ENC);
				break;
			}
			case CHR:{
				char* s = data;
				temp[0] = encStr(*s);
				temp[1] = '\0';
				break;
			}
			case DOUBLE:
			{
				double* tmp = data;
				sprintf(temp, "%lf", *tmp);
				edNum(temp, ENC);
				break;
			}
			default: {
				fclose(rv);
				fseek(rd, offset, 0);
				*errnum = -4;
				return;
			}
		}
	fprintf(rv, "%s",  temp);
	fseek(rd, pos2, 0);
	while((y = getc(rd))!= -1)putc(y, rv);
	fclose(rv);
	fclose(rd);
	rename(n, tempFile);
	*errnum = 0;
	rd = fopen(tempFile, "r+");
	fseek(rd, offset, 0);
	return;
}
/**
 * This method is used to get Data format.
 * The data format is in an Array or not array
 * the way to use is same with getIDElem()
 * usage :
 * getTPElem(filename, signature, name_element);
 */
int getTPElem(PDATA* pdata, const char *elem){
	if(pdata == NULL)return -1;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	off_t offset = ftell(rd);
	int bl = checkExistsElem(temp, rd, elem, offset);
	if(!bl) {
		fseek(rd, offset, 0);
    	return (*errnum = -2);
	}
	char ID = getc(rd);
	char TP = getc(rd);
	fseek(rd, offset, 0);
	*errnum=0;
	return TP;
}
/**
 * This method is used to get an unique data an element
 * to show the type data, you should comparing this to struct of Data
 * usage :
 * getIDElem(nameElements);
 * and to show the data type, must compare with struct Data
 * example
 * if(getIDElem("CobaInt") == INT)){
 *	 //your statement goes here
 * }

 */
int getIDElem(PDATA* pdata, const char *elem){
	if(pdata == NULL)return -1;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	off_t offset = ftell(rd);
	int bl = checkExistsElem(temp, rd, elem, offset);
	if(!bl) {
		fseek(rd, offset, 0);
    	return (*errnum = -2);
	}
	char ID = getc(rd);
	fseek(rd, offset, 0);
	*errnum = 0;
	return ID;
}
/*** String Array ***/

/* int addStrArr(char file[], char sig[], char elem[], char *str[], size_t len)
 * This method is used to add an array of string
 * the array that contains a string data types.
 * the each string content element,
 * cannot exceed the max value of the collection of chars
 * the max buffer of string/max collection character allowed is
 * 32768 chars or 32Kb chars
 * because of security and hardware, you can't exceed this. and you are allowed to
 * below max buffer.
 * usage :
 * addStrArr(nameElementToAdd, array_of_strings, length_of_strings);
 */
void addStrArr(PDATA* pdata, const char *elem, const char **str, size_t len){
	if(pdata == NULL) return;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	off_t offset = ftell(rd);
	if(checkExistsElem(temp, rd, elem, offset)){
		fseek(rd, offset, 0);
		*errnum = -2;
		return;
	}
	// writing first content
	fprintf(rd, "%c%c%s%c", ID_ARR, STR, elem, BUKA);
	// writing content array
	size_t x = 0;
	for (; x < len; x++){
		strcpy(temp, str[x]);
		edStr(temp, ENC);
		__passToFile(temp, rd, x, len);
	}
	// close
    fseek(rd, offset, 0);
	*errnum = 0;
    return ;
}
/** char* getStrAtPos(char elem[], size_t pos);
 * This method is used to get the string in its array
 * after you assign its function into a array of char
 * you must copy array value into your own array of char.
 * because this function use the static temporary array of chars
 * this temporary is used to assign the value before returning
 * so you can't kept the using
 * usage :
 * getStrAtPos(nameElements, position_in_array);
 */
char* getStrAtPos(PDATA* pdata, const char *elem, size_t pos){
	if(pdata == NULL)return NULL;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)){
		fseek(rd, offset, 0);
		*errnum = -2;
		return NULL;
	}
	int ID = getc(rd);
	int TP = getc(rd);
	if(ID != ID_ARR || TP != STR){
		fseek(rd, offset, 0);
		*errnum = -3;
		return NULL;
	}
	int c, y  = 0;
	while ((c = getc(rd))!=BUKA);
	off_t pos1 = ftell(rd);
	size_t sizeArr = count(rd, pos1);
	if(pos < 0 || pos >= sizeArr){
		fseek(rd, offset, 0);
		*errnum = -4;
		return NULL;
	}
	getData(temp, rd, pos);
	edStr(temp, DEC);
	fseek(rd, offset, 0);
	*errnum = 0;
	return temp;
}
/** int appendStrArr(char elem[],  char *str[], size_t len)
 * This method is used to adding the array of string into at the end of elementArray
 * with the specified length of array string
 * usage :
 * appendStrArr(nameElement, string_array, length_of_array)
 */
void appendStrArr(PDATA* pdata, const char *elem, const char **str, size_t len){
	if(pdata == NULL)return;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	char* tempFile = pdata->tempFile;
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)) {
		fseek(rd, offset, 0);
		*errnum = -2;
		return;
	}
	char ID = getc(rd);
	char TP = getc(rd);
	if (ID != ID_ARR || TP != STR){
		fseek(rd, offset, 0);
		*errnum = -3;
		return;
	}
	register int y;
	while ((y = getc(rd))!=TUTUP);
	off_t pos1 = ftell(rd)-1;
	char n[32];
	sprintf(n, "%s.tmp", tempFile);
	FILE* rv = fopen(n, "w+");
	fseek(rd, pos1, 0);
	// pointing into before TUTUP Character Element
	// because for easy to append
	/*for (off_t x = 0; x < pos1-1 && ((y = getc(read))!=-1); x++)
		putc(y, rv);*/
	size_t x = 0;
	for(; x<len; x++){
		strcpy(temp, str[x]);
		edStr(temp, ENC);
		fprintf(rv, "%c%s", ARR_SEPARATOR,temp);
	}
	while((y = getc(rd))!=-1) putc(y, rv);
	fseek(rd, pos1, 0);
	fseek(rv, 0, 0);
	while((y = getc(rv))!=-1)putc(y, rd);
	fclose(rv);
	remove(n);
	fseek(rd, offset, 0);
	*errnum = 0;
	return ;
}
/**
 * void setStrArr(char elem[], char *str[], size_t len)
 * This method is used to change all content of array with the new content in array of string str
 * and with length of array
 * usage :
 * setStrArr(nameOfElements, array_of_strings, length_of_array
 */
void setStrArr(PDATA* pdata, const char *elem, const char **str, size_t len){
	if (pdata == NULL)return;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	char* tempFile = pdata->tempFile;
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)) {
		fseek(rd, offset, 0);
		*errnum = -2;
		return;
	}
	char ID = getc(rd);
	char TP = getc(rd);
	if (ID != ID_ARR || TP != STR){
		fseek(rd, offset, 0);
		*errnum = -3;
		return;
	}
	register int y;
	while ((y = getc(rd))!=BUKA);
	off_t pos1 = ftell(rd);
	while ((y = getc(rd))!=TUTUP);
	off_t pos2 = ftell(rd);
	char n[32];
	sprintf(n, "%s.tmp", tempFile);
	FILE* rv = fopen(n, "w+");
	fseek(rd, 0,0);
	off_t t = 0;
	while(((y = getc(rd))!= -1)&&(t++ < pos1))putc(y, rv);
	size_t x = 0;
	for(; x<len; x++){
		strcpy(temp, str[x]);
		edStr(temp, ENC);
		fprintf(rv, "%s%c",  str[x], ARR_SEPARATOR);
	}
	t  = ftell(rv);
	fseek(rv, t-1, 0);
	fseek(rd, pos2-1, 0);
	while(((y = getc(rd))!= -1))putc(y, rv);
	fclose(rd);
	fclose(rv);
	//printf("hello");
	rename(n , tempFile);
	rd = fopen(tempFile, "r+");
	fseek(rd, offset, 0);
	*errnum = 0;
	return;
}
/**
 * void setStrArrAtPos(char elem[], char *str, size_t pos)
 * This method is used to change str elements
 * at the position in data string element
 * usage :
 * setStrArrAtPos(nameElements, new_string_value, position_to_change)
 */
void setStrArrAtPos(PDATA* pdata, const char *elem, const char *str, size_t pos){
	if( pdata == NULL)return;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	char* tempFile = pdata->tempFile;
	off_t offset = ftell(rd);
	int bl = checkExistsElem(temp, rd, elem, offset);
	if(!bl) {
		fseek(rd, offset, 0);
		*errnum = -2;
		return;
	}
	char ID = getc(rd);
	char TP = getc(rd);
	if (ID != ID_ARR || TP != STR){
		fseek(rd, offset, 0);
		*errnum = -3;
		return;
	}
	register int y;
	while ((y = getc(rd))!=BUKA);
	off_t pos1 = ftell(rd), pos2;
	size_t lnArr = count(rd, pos1);
	if( pos < 0 || pos >= lnArr){
		fseek(rd, offset, 0);
		*errnum = -4;
		return;
	}
//	printf("%c", getc(read));
	//printf("%d, %d", pos1, pos2);
	__pointToElem(rd, &pos1, &pos2, pos);
	char n[32];
	sprintf(n, "%s.tmp", tempFile);
	FILE* rv = fopen(n, "w+");
	fseek(rd, 0, 0);
	off_t t = 0;
	while(((y = getc(rd))!= -1)&&(t++ < pos1))putc(y, rv);
	strcpy(temp, str);
	edStr(temp, ENC);
	fprintf(rv, "%s",  temp);
	fseek(rd, pos2, 0);
	while((y = getc(rd))!= -1)putc(y, rv);
	fclose(rv);
	fclose(rd);
	rename(n, tempFile);
	rd = fopen(tempFile, "r+");
	fseek(rd, offset, 0);
	*errnum = 0;
	return;
}
/**char **getStrArr(char elem[])
 * This merhod is Deprecated
 * because this method can cause memory leaks or
 * causing crash or consuming more heavy memory
 * so, I assume you don't use this method
 * usage : isn't available because Deprecated
 */
char **getStrArr(PDATA* pdata, const char *elem){
	if(pdata == NULL) return NULL;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)){
		fseek(rd, offset, 0);
		*errnum = -2;
		return NULL;
	}
	// get Identifier
	int ID = getc(rd);
	// get TipeData
	int TP = getc(rd);
	if(ID != ID_ARR || TP != STR){
		fseek(rd, offset, 0);
		*errnum = -2;
		return NULL;
	}
	int c, y  = 0;
	// Pointing the offset to BUKA char
	while ((c = getc(rd))!=BUKA);
	// save offset
	off_t pos1 = ftell(rd);
	size_t cnt = count(rd, pos1);
		char tmp[cnt][32768];
		int z =0, z2 = 0;
		while(1){
			y = getc(rd);
			if (y == ARR_SEPARATOR){
				temp[z] = '\0';
				z = 0;
				edStr(temp, DEC);
				strcpy(tmp[z2++], temp);
				//printf("%s\n", temp);

			}
			else if(y == TUTUP){
				temp[z] = '\0';
				z = 0;
				edStr(temp, DEC);
				strcpy(tmp[z2++], temp);
				//printf("%s\n", temp);
				break;
			}
			else temp[z++] = y;
		}
		char **x = (char**)tmp;
		fseek(rd, offset, 0);
		*errnum = 0;
		//printf("finished!");*/
        return x;
}
size_t getArrLength(PDATA* pdata, const char *elem){
	if(pdata == NULL) return -1;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)){
		fseek(rd, offset, 0);
		return (*errnum = -2);
	}
	// get Identifier
	int ID = getc(rd);
	// get TipeData
	int TP = getc(rd);
	if(ID != ID_ARR){
		fseek(rd, offset, 0);
		return (*errnum = -2);
	}
	int c, y  = 0;
	// Pointing the offset to BUKA char
	while ((c = getc(rd))!=BUKA);
	off_t pos1 = ftell(rd);
	size_t cnt = count(rd, pos1);
	fseek(rd, offset, 0);
	*errnum = 0;
	return cnt;
}
size_t findPos(PDATA* pdata, const char *elem, void* data){
	if(pdata == NULL) return -1;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	// Getting array length
	size_t arrLen = getArrLength(pdata, elem);
	if(arrLen < 0) return *errnum = -1;
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)){
		fseek(rd, offset, 0);
		return (*errnum = -2);
	}
	// get Identifier
	int ID = getc(rd);
	// get TipeData
	int TP = getc(rd);
	if(ID != ID_ARR){
		fseek(rd, offset, 0);
		return (*errnum = -3);
	}
	/* Converting ALL to STR (NUM)*/
	char *str;
	switch(TP){
		case STR:
			str = data;
			break;
		case INT:
		{
			static char i[512];
			int* in = data;
			sprintf(i, "%d", *in);
			str = i;
		}
		break;
		case LONG:
		{
			static char i[512];
			long long int* in = data;
			sprintf(i, "%lld", *in);
			str = i;
		}
		break;
		case DOUBLE:
		{
			static char i[512];
			double* in = data;
			sprintf(i, "%lf", *in);
			str = i;
		}
		break;
		case CHR:
		{
			static char i[2];
			char* in = data;
			sprintf(i, "%c", *in);
			str = i;
		}
		break;
	}

	size_t pos = 0;
	register int chr, lock = 0;
	//pointing to buka
	while((chr = getc(rd))!=BUKA);
	int x = 0, y =0;
	for(;pos < arrLen;){
		if(lock == 0)
			while(1){
				chr = getc(rd);
		    	if(chr == TUTUP || chr == ARR_SEPARATOR){
		    		temp[y] = '\0';
		    		lock = 1;
					y = 0;
		    		break;
		    	}
		    	else temp[y++] = chr;
			}
		else if(lock == 1){
			//convert to str and decoding
			switch(TP){
				case INT:
				case DOUBLE:
				case LONG:
					edNum(temp, DEC);
				break;
				case CHR:
				{
					char s = temp[0];
					temp[0] = decStr(s);
				}
				break;
				case STR:
					edStr(temp, DEC);
				break;
			}
			//checking string
			// if true break current loop
			// if false lock = 0
			//printf("temp = %s\nstr = %s\n", temp, str);
			int bl = cmp(temp, str);
			if(bl)
				break;
			else{
				lock = 0;
				pos++;
			}
		}
	}
	fseek(rd, offset, 0);
	if(pos >= arrLen)
		return *errnum = -3;
	return pos;
}
/**
 * char *readStringPoint(char elem[])
 * This method is used to get the String element in array
 * with directly point to the string element
 * to get one by one. used by loop to get all content
 * strings. This method is usefull than getStrArr(elem)
 * because this method use less memory than getStrArr(elem)
 * and cause no longer times to get all string than getStrArr(elem)
 * but after calling this method, you should call
 * resetPoint() to resetting pointer to beggining.
 */
char *readStringPoint(PDATA* pdata, const char *elem){
	if(pdata == NULL)return NULL;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	int* lockPointer = &(pdata->lockPointer);
	off_t* offt = &(pdata->offt);
	off_t* offset = &(pdata->sigOffset);
	register int y, z = 0;
	if(*lockPointer == -1){
		if(!checkExistsElem(temp, rd, elem, *offset)){
			fseek(rd, *offset, 0);
			*errnum = -2;
			return NULL;
		}
		// get Identifier
		int ID = getc(rd);
		// get TipeData
		int TP = getc(rd);
		if(ID != ID_ARR || TP != STR){
			fseek(rd, *offset, 0);
			*errnum = -2;
			return NULL;
		}
		// Pointing the offset to BUKA char
		while ((y = getc(rd))!=BUKA);
		while(1){
			y = getc(rd);
			if (y == ARR_SEPARATOR){
				temp[z] = '\0';
				z = 0;
				edStr(temp, DEC);
				*lockPointer = 0;
				break;
			}
			else if(y == TUTUP){
				temp[z] = '\0';
				z = 0;
				edStr(temp, DEC);
				*lockPointer = 1;
				break;
			}
			else temp[z++] = y;
		}
		char *x = temp;
		*errnum = 0;
		*offt = ftell(rd);
		fseek(rd, *offset, 0);
		return x;
	}
	else if(*lockPointer == 0){
		fseek(rd, *offt, 0);
		while(1){
			y = getc(rd);
			if (y == ARR_SEPARATOR){
				temp[z] = '\0';
				z = 0;
				edStr(temp, DEC);
				break;
			}
			else if(y == TUTUP){
				temp[z] = '\0';
				z = 0;
				edStr(temp, DEC);
				*lockPointer = 1;
				break;
			}
			else temp[z++] = y;
		}
		char *x = temp;
		*errnum = 0;
		int dist = ftell(rd) - (*offt);
		*offt += dist;
		fseek(rd, *offset, 0);
		return x;
	}
	else{
		*errnum = 1;
	    return NULL;
	}
}
/*
 * void resetPoint()
 * This method is used to resetting pointer after
 * calling readStringPoint(elem) to the beginning area
 */
void resetPoint(PDATA* pdata){
	if(pdata == NULL)return;
	// Copying pointers from pdata to temp and rdfiles
	int* lockPointer = &(pdata->lockPointer);
	int* errnum = &(pdata->errnum);
	off_t* offt = &(pdata->offt);
	if(*lockPointer == -1)return;
	*offt = 0;
}
int isElemExists(PDATA* pdata, const char *elem)
{
    if(pdata == NULL) return -1;
	// Copying pointers from pdata to temp and rdfiles
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	// Getting array length
	size_t arrLen = getArrLength(pdata, elem);
	if(arrLen < 0) return *errnum = -1;
	off_t offset = ftell(rd);
	if(!checkExistsElem(temp, rd, elem, offset)){
		fseek(rd, offset, 0);
		return 0;
	}
	fseek(rd, offset, 0);
	return 1;
}
char *readNameElem(PDATA* pdata){
	FILE* rd = pdata->rd;
	char* temp = pdata->temp;
	int* errnum = &(pdata->errnum);
	int* lockPointer = &(pdata->lockPointer);
	off_t* offt = &(pdata->offt);
	off_t* offset = &(pdata->sigOffset);
	register int x, y;
	
	if(*lockPointer == -1){
		fseek(rd, *offset, 0);
		//pointing into ID ELEMENT
		while((x = getc(rd)) != -1 && x != ID && x != ID_ARR);
		//point to next identifier name
		if(x == -1){
			*lockPointer = 1;
			*errnum = -1;
			return NULL;
		}
		getc(rd);
		y = 0;
		//read name elements
		while((x = getc(rd)) != BUKA) temp[y++] = x;
		temp[y] = '\0';
		// point to TUTUP element
		while((x = getc(rd)) != TUTUP);
		// saving offset
		*offt = ftell(rd);
		// set into sigOffset
		fseek(rd, *offset, 0);
		// change lockPoint into zero
		*lockPointer = 0;
		// return result
		*errnum = 0;
		return temp;
	}
	else if(*lockPointer == 0){
		fseek(rd, *offt, 0);
		//pointing into ID ELEMENT
		while((x = getc(rd)) != -1 && x != ID && x != ID_ARR);
		//point to next identifier name
		if(x == -1){
			*lockPointer = 1;
			*errnum = -1;
			return NULL;
		}
		getc(rd);
		y = 0;
		//read name elements
		while((x = getc(rd)) != BUKA) temp[y++] = x;
		temp[y] = '\0';
		// point to TUTUP element
		while((x = getc(rd)) != TUTUP);
		// saving offset
		*offt = ftell(rd);
		// set into sigOffset
		fseek(rd, *offset, 0);
		// change lockPoint into zero if next not null
		if(getc(rd) == -1)
			*lockPointer = 1;
		else
			*lockPointer = 0;
		// return result
		*errnum = 0;
		return temp;
	}
	else {
		*errnum = -1;
		return NULL;
	}
}
