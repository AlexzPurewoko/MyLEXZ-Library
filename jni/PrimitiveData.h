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
 *	@(#)PrimitiveData.h	v5 @@ HEADER FILES @@
 * Last updated in : Friday Dec, 1, 2017 in 04:57 PM
 * Time Zone : Jakarta, GMT+7 , WIB
 */

 /* This file is used to declare a functions on PrimitiveData.c
 */
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
#ifndef PRIMITIVEDATA_H
#define PRIMITIVEDATA_H

//declare specific libraries
#define _DEF_TYPES_
#include "component.h"
/**
* DON'T MODIFY THIS FILE!! OR YOU HAVE TO GETTING AN ERROR
*/
//enum SYM -> For Symbol identifier
static enum SYM {
	BUKA 			= 0x002,
	TUTUP 			= 0x001,
	ID 				= 0x004,
	ID_ARR 			= 0x006,
	ARR_SEPARATOR 	= 0x003
}SYM;
// Data structures, DON'T EDIT!!
typedef struct PData {
	/*
	* This variable is used to collecting the character to improve the ability.
	* Because for simple editing, and writing.
	* allocate 32Kb for RAM.
	* This is safe
	* and static variable, can save the variables at a long time
	* and while program is closing , this variable can appear in RAM at time.
	* and automatically deleted by gc
	*/
	char temp[32768];
	/**
	 * This allocation used to store temp variable
	 * from where inputed as char file[] in open() methods
	 * The function is to save local file path
 	 */

	char tempFile[1024];
	/**
	 * this field is used to store errors while
	 * after calling a function
	 * if no errors, this value always be a zero
	 */
	int errnum;
	/**
 	 * this variable is a part of readStringPoint
 	 * used to define if lockPointer = -1 is beginning
	 * operation to get first value of string array
 	 * if lockPointer = 0 is continue the first operation, second
 	 * operation, n-operation untill reaching the TUTUP char
 	 * if lockPointer = 1 tell the operation will return NULL because
 	 * allready done raise the TUTUP char, so to reset this var
 	 * you must call resetPoint();
 	 *
 	 */
	int lockPointer;
	/**
 	 * this variable is used to save the offset value before
 	 * starting the operation in readStringPoint(elem) and this
	 * variable is used by resetPoint() to resetting offset file
 	 * to the offt
 	 */
	off_t offt;
	/**
	 * Signature end offset
	 */
	off_t sigOffset;
	
	/**
	 * this field used to store a temp file
	 * first value is NULL
 	 */
	FILE* rd;
}PDATA;
/*****  This is a function to support the programming of files  ***/
PDATA* openf(const char *file, const char *sig);
/**
 * method to close a file and releasing another
 * resources that used to programming the file
 */
void closef(PDATA* data);
/*
 * method to getting value error from errnum
 * the field variables is hidden because to protect and
 * to avoid many problems
 */
int getErrors(PDATA* data);
/**
 * find element data position in an array
 * at the first left value in array that have been
 * compared successfull (true)
 */
size_t findPos(PDATA* pdata, const char *elem, void* data);
/**
 * Getting array length at specified element
 */
size_t getArrLength(PDATA* pdata, const char *elem);
/**
 * addElem(filename, signature, element, tipeData<int>, data);
 * used to add element with specified data and tipeData to
 * declare a data.
 */
void addElem(PDATA* pdata, const char *elem, int tpData, void* data);
/**
 * deleteElem(filename, signature, nameElement);
 * deleting Elements at specified nameElement
 */
void deleteElem(PDATA* data, const char *elem);
/**
 * getElem(filename, signature, nameElement);
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
void* getElem(PDATA* data, const char *elem);
/**
 * setElem(filename, signature, nameElements, content_data)
 * Setting data element at nameElements
 * By changing old data at elements to new data value
 */
void setElem(PDATA* data, const char *elem, void* reserved);
/**
 * addArr(filename, signature, nameElement, TipeData, content_data, length_data);
 * Add Array Element in file
 * This method isn't compatible if tpData == STR
 * if you would like to use arrayStrings
 * you must call addStrArr() function
 */
void addArr(PDATA* pdata, const char *elem, int tpData, void* data, size_t dataS);
/**
 * getAllContentInArr(filename, signature, elements);
 * This method is used to get All Content in Array
 * or get array of an element.
 * Not for STR ARRAY!!
 */
void* getAllContentInArr(PDATA* pdata, const char *elem);
/**
 * This method is used to get data at the position in elementArray
 * usage :
 * getContentArrAtPos(nameElement, position_data_in_array<int>);
 */
void* getContentArrAtPos(PDATA* pdata, const char *elem, size_t pos);
/**
 * This method is used to append a content of an array at the last
 * position in the elementArray
 * usage :
 * appendArr(nameElement, arrayData, length_array_to_add):
 */
void appendArr(PDATA* pdata, const char *elem, void* data, size_t len);
/**
 * This method is used to set an arrat by changing all elements
 * and take the new value of the element
 * usage :
 * setArr(nameElements, contentElements, lengthOfElement)
 */
void setArr(PDATA* pdata, const char *elem, void* data, size_t len);
/**
 * This method is used to setting data element at position in
 * an array
 * usage :
 * setDataAtPosArr(nameElements, elementType<struct data>, contentToChange, positionToChange)

 */
void setDataAtPosArr(PDATA* pdata, const char *elem, int elemType, void* data, size_t pos);
/**
 * This method is used to get Data format.
 * The data format is in an Array or not array
 * the way to use is same with getIDElem()
 * usage :
 * getTPElem(name_element);
 */
int getTPElem(PDATA* pdata, const char *elem);
/**
 * This method is used to get an unique data an element
 * to show the type data, you should comparing this to struct of Data
 * usage :
 * getIDElem(filename, signature_file, nameElements);
 * and to show the data type, must compare with struct Data
 * example
 * if(getIDElem(file, sig, "CobaInt") == INT)){
 *	 //your statement goes here
 * }

 */
int getIDElem(PDATA*pdata, const char *elem);

/* function for string array */
/* void addStrArr(char elem[], char *str[], size_t len)
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
void addStrArr(PDATA* pdata, const char *elem, const char **str, size_t len);
/** int getStrAtPos(char elem[], size_t pos);
 * This method is used to get the string in its array
 * after you assign its function into a array of char
 * you must copy array value into your own array of char.
 * because this function use the static temporary array of chars
 * this temporary is used to assign the value before returning
 * so you can't kept the using
 * usage :
 * getStrAtPos(nameElements, position_in_array);
 */
char* getStrAtPos(PDATA* pdata, const char *elem, size_t pos);
/**char **getStrArr(char elem[])
 * This merhod is Deprecated
 * because this method can cause memory leaks or
 * causing crash or consuming more heavy memory
 * so, I assume you don't use this method
 * usage : isn't available because Deprecated
 */
char **getStrArr(PDATA* pdata, const char *elem);
/** int appendStrArr(char file[], char sig[], char elem[],  char *str[], size_t len)
 * This method is used to adding the array of string into at the end of elementArray
 * with the specified length of array string
 * usage :
 * appendStrArr(filename, signature, nameElement, string_array, length_of_array)
 */
void appendStrArr(PDATA* pdata, const char *elem, const char **str, size_t len);
/**
 * void setStrArr(char elem[], char *str[], size_t len)
 * This method is used to change all content of array with the new content in array of string str
 * and with length of array
 * usage :
 * setStrArr(nameOfElements, array_of_strings, length_of_array
 */
void setStrArr(PDATA* pdata, const char *elem, const char **str, size_t len);
/**
 * void setStrArrAtPos(char elem[], char *str, size_t pos)
 * This method is used to change str elements
 * at the position in data string element
 * usage :
 * setStrArrAtPos(nameElements, new_string_value, position_to_change)
 */
void setStrArrAtPos(PDATA* pdata, const char *elem, const char *str, size_t pos);
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
char *readStringPoint(PDATA* pdata, const char *elem);
/*
 * void resetPoint()
 * This method is used to resetting pointer after
 * calling readStringPoint(elem) to the beginning area
 */
void resetPoint(PDATA* pdata);
int isElemExists(PDATA* pdata, const char *elem);
char *readNameElem(PDATA* pdata);
#endif /* PRIMITIVEDATA_H */
