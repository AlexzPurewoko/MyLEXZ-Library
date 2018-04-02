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
 *	@(#)Log.c	v2 @@ SOURCE FILE @@
 * Last updated in : Tuesday Nov, 23, 2017 in 05.31 PM
 * Time Zone : Jakarta, GMT+7 , WIB
 */
#include "log.h"

char sig[] = "CLogger";

//static enum { ENC, DEC} MODE;
/* Function of collecting Time and date */
/*******************************/

/* CONSTANT EXPRESSION */

static enum {
	_BUKA_ = 0x02,
	_TUTUP_ = 0x01,
	PEMISAH = 0x03
} LOGSYM;
/******/
/*********** Function For encrypt and decrypt ************/
int estr(int x){
	return x + ((1<<7)-(1<<2));
}
// Decryptor for String
int dstr(int x){
	return x - ((1<<7)-(1<<2));
}
int enm(int d){
	return ( ~ ( ~ ( d << 1 ) + ~0 + ~('0' >> 3)) + ((d - '0')+(1<<3)<<3))-(5 << 1);
	
}
int dnum(int e){
	return (((e + (5 << 1))- (1 << 3)) / ( 5 << 1)) + (1 << 5);//-(5 << 1);
}

/***********************************/

void check(int *tochck, int val, int *hchk){
	while((*tochck)> val){
		(*tochck )-=(val+1);
		(*hchk)++;
	}
}
void totime(unsigned long int  millis, struct __time *v){
	v->_millis = millis % 1000;
	millis/=1000;
	v->_sec = millis%100;
	millis-=v->_sec;
	millis/=60;
	v->_min = millis%60;
	v->_hours = millis/60;
	// arg check for time
	//check minutes
	check(&(v->_sec), 59, &(v->_min));
	//check hours
	check(&(v->_min), 59, &(v->_hours));
}
void todate(unsigned long int date, struct __date *x){
	x->_years = date%10000;
	date/=10000;
	x->_month = date%100;
	date/=100;
	x->_day = date%100;
	date/=100;
}
/**
 * edNum( String_of_number_to_enc_or_dec, mode_dec_or_enc);
 * Encrypt or Decrypt the String of number into its string again
 * without return
 */
void _edNum(char num[], int mode){
	reverse(num);
	size_t len = strlen(num), x = 0;
	register int chr;
	for (; x < len; x++){
		if(mode == ENC)
		    chr = enm(num[x]);
		else if(mode == DEC)
		    chr = dnum(num[x]);
		num[x] = chr;
	}
}

// Encryptor for number
/**
 * edStr( String_to_enc_or_dec, mode_dec_or_enc);
 * Encrypt or Decrypt the String into its string again
 * without return
 */
void _edStr(char str[], int mode){
	reverse(str);
	size_t len = strlen(str), x = 0;
	register int chr;
	for (; x < len; x++){
		if( mode == ENC)
		    chr = estr(str[x]);
		else if(mode == DEC)
		    chr = dstr(str[x]);
		str[x] = chr;
	}
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
int _checkSignature(LOG* log){
	
	if(log->rdL == NULL)return 0;
	int elem, chr;
	for(elem = 0; (chr = getc(log->rdL)) != '\n' && chr != -1; elem++)log->_logBuff[elem] = chr;
	if(chr == -1)return 0;
	log->_logBuff[elem] = '\0';
	//printf("%d",cmp(temp,signature));
    _edStr(log->_logBuff, DEC);
	int bol = cmp(log->_logBuff, sig);
	if(bol)
		return ++elem;
	else
		return 0;
}

/*
 * Write a new Logger Format file using signature
 * must using signature!
 * usage :
 * writeNew(filename, signature);
 */
void _writeNew(LOG* log, char file[]){
	FILE* fj = fopen(file, "w+");
	strcpy(log->_logBuff, sig);
	printf("%s\n", log->_logBuff);
	_edStr(log->_logBuff, ENC);
	fprintf(fj, "%s\n", log->_logBuff);
	fclose(fj);
	return;
}
long long atol64(const char s[]){
	int x = 0;
	long long b = 0;
	for(;s[x] != '\0'; x++)
	    b = (b +( s[x] - '0')) * 10;
	return b/10;
}
long buildDate(const struct __date* mDate){
	return (mDate->_day * 1000000)+(mDate->_month * 10000)+mDate->_years;
}
long buildTime(const struct __time* mTime){
	return (mTime->_hours *3600000)+(mTime->_min * 60000)+(mTime->_sec * 1000) + mTime->_millis;
}
void getCurrentDateAndTimes(struct __time* mtime, struct __date* date){
	struct timeb now;
	//get current times and passing to 'now'
	ftime(&now);
	//convert the arithmetical time into struct tm
	struct tm* listTime = localtime(&now.time);
	// Passing into list of date and time
	mtime->_millis = now.millitm; //milliseconds
	mtime->_sec    = listTime->tm_sec; //seconds
	mtime->_min    = listTime->tm_min; //minutes
	mtime->_hours   = listTime->tm_hour; //Hour
	
	date->_day    = listTime->tm_mday; //Date day
	date->_month  = listTime->tm_mon + 1; // Month of year
	date->_years  = listTime->tm_year + 1900; // The Current Year
}
/******* BASIC FUNCTION OF PROGRAM ******/
/**
 * used to open a file and placing into
 * temporary static file
 * open(filename);
 */
LOG* openl(char file[], char process[]){
	//Checking files.....
//	FILE* b = log->rdL;
//	if(b  != NULL)return;
	// make new LOG
	LOG* log = malloc(sizeof(LOG));
	if(checkExists(file))_writeNew(log, file);

	//Check Signature
	log->rdL = fopen(file, "r+");
	int offset = _checkSignature(log);
	if(!offset){
		fclose(log->rdL);
		log->rdL = NULL;
	    log->_log_err = -1;
		return log;
	}
	strcpy(log->_logFile, file);
//	printf("%s\n", log->_logFile);
	fseek(log->rdL, offset, 0);
	log->_log_err = 0;
	log->_lockPointer = -1;
	strcpy(log->proc, process);
	return log;
}
void writeLog(LOG* log, struct __logs* data){
	if(log->rdL == NULL)return;
	char tm[10], dt[10];
	long Time = buildTime(&data->mTime);
	long Date = buildDate(&data->mDate);
	sprintf(tm, "%ld", Time);
	sprintf(dt, "%ld", Date);
	// Enkripsi semua
	_edNum(tm, ENC);
	_edNum(dt, ENC);
	_edStr(data->proc_name, ENC);
	_edStr(data->tag, ENC);
	_edStr(data->content, ENC);
	register int chr;
	while((chr = getc(log->rdL))!=-1);
	// write ke file
	fprintf(log->rdL, "%c%c%c%s%c%s%c%s%c%s%c%s%c", 
	_BUKA_, data->prio, PEMISAH, data->proc_name, PEMISAH, data->tag, PEMISAH, dt, PEMISAH, tm, PEMISAH, data->content, _TUTUP_);
	log->_log_err = 0;
//	edStr(data->proc_name, DEC);
	return;
}
void setProccess(LOG* log, const char *proccess){
	strcpy(log->proc, proccess);
	log->_log_err = 0;
	return;
}
void writeL(LOG* log, int priority, char *tagS, char *content, ...){
	// get Current times
	struct __time time;
	struct __date date;
	getCurrentDateAndTimes(&time, &date);
	va_list args;
	if(log->proc == NULL){
		log->_log_err = 1;
		return;
	}
	va_start(args, content);
	vsprintf(log->_logBuff, content, args);
	log->current.mTime = time;
	log->current.mDate = date;
	log->current.prio = priority;
	strcpy(log->tag, tagS);
	log->current.tag = log->tag;
	log->current.content = log->_logBuff;
	strcpy(log->current.proc_name, log->proc);
	writeLog(log, &log->current);
	va_end(args);
	log->_log_err = 0;
	return;
}
int logError(LOG* log){
	return log->_log_err;
}
void read_and_pass(LOG* flog, struct __logs* log){
	register int x, y = 0;
	if(flog->_lockPointer == -1)
	{
		fseek(flog->rdL, 0, 0);
		// Pointing to BUKA element
		while((x = getc(flog->rdL)) != _BUKA_);
	}
	x = getc(flog->rdL);// priority
	log->prio = x;
	getc(flog->rdL);// PEMISAH
	while((x = getc(flog->rdL))!= PEMISAH)
		log->proc_name[y++] = x;
	log->proc_name[y] = '\0';
	_edStr(log->proc_name, DEC);
	y = 0;
	while((x = getc(flog->rdL))!= PEMISAH)
		flog->tag[y++] = x;
	flog->tag[y] = '\0';
	_edStr(flog->tag, DEC);
	y = 0;
	//Dates
	char mDate[10];
	while((x = getc(flog->rdL))!= PEMISAH)
		mDate[y++] = x;
	mDate[y] = '\0';
	_edNum(mDate, DEC);
	long lDate = atol(mDate);
	y = 0;
	//Times
	char mTime[10];
	while((x = getc(flog->rdL))!= PEMISAH)
		mTime[y++] = x;
	mTime[y] = '\0';
	_edNum(mTime, DEC);
	long lTime = atol(mTime);
	y = 0;
	//convertTime and date into struct and passing it
	totime(lTime, &log->mTime);
	todate(lDate, &log->mDate);
	while((x = getc(flog->rdL))!=_TUTUP_)
		flog->_logBuff[y++] = x;
	flog->_logBuff[y] = '\0';
	y = 0;
	_edStr(flog->_logBuff, DEC);
	//copying tag and content into list
	log->content = flog->_logBuff;
	log->tag = flog->tag;
	
}
struct __logs* readL(LOG* flog){
	if(flog->rdL == NULL)return NULL;
	struct __logs* log = &flog->current;
	if(flog->_lockPointer == -1){
		read_and_pass(flog, log);
		if(getc(flog->rdL) == -1)flog->_lockPointer = 1;
		else
		    flog->_lockPointer++;
		flog->_log_err = 0;
		return log;
	}
	else if(flog->_lockPointer == 0){
		read_and_pass(flog, log);
		if(getc(flog->rdL) == -1)flog->_lockPointer = 1;
		flog->_log_err = 0;
		return log;
	}
	else {
		flog->_log_err = 1;
		log->prio = -1;
		return log;
	}
}

void readS(LOG* flog, char res[]){
	struct __logs* rlog = readL(flog);
	char priority[7];
	switch(rlog->prio){
		case ERRORS  :
			strcpy(priority, "ERRORS");
			break;
		case INFO	:
			strcpy(priority, "INFO");
			break;
		case DEBUG 	:
			strcpy(priority, "DEBUG");
			break;
		case VERBOSE :
			strcpy(priority, "VERBOSE");
			break;
		case WARNING :
			strcpy(priority, "WARNING");
			break;
		case FAILURE :
			strcpy(priority, "FAILURE");
			break;
		case -1:
		{
			flog->_log_err = 1;
			res = NULL;
			return;
		}
	}
	sprintf( res, "%d/%d/%d\t%d:%d:%d.%d\t%s\t%s\t%s\t%s",
	rlog->mDate._day, rlog->mDate._month, rlog->mDate._years,
	rlog->mTime._hours, rlog->mTime._min, rlog->mTime._sec, rlog->mTime._millis,
	priority, rlog->proc_name, rlog->tag, rlog->content);
	flog->_log_err = 0;
}
void reset(LOG* flog){
	if(flog->_lockPointer == 0 || flog->_lockPointer == 1)
	{
		fseek(flog->rdL, 0, 0);
		flog->_lockPointer = -1;
	}
}
struct __logs* read_withPrio(LOG* flog, int priority){
	struct __logs* data;
	while(1){
		data = readL(flog);
	 	if(data->prio == -1&&flog->_lockPointer == 1){
			flog->_log_err = 1;
			return data;
	 	}
		else if(data->prio == priority)break;
	}
	return data;
}
void readS_withPrio(LOG* flog, int prio, char res[]){
	struct __logs* rlog = read_withPrio(flog, prio);
	char priority[7];
	switch(rlog->prio){
		case ERRORS  :
			strcpy(priority, "ERRORS");
			break;
		case INFO	:
			strcpy(priority, "INFO");
			break;
		case DEBUG 	:
			strcpy(priority, "DEBUG");
			break;
		case VERBOSE :
			strcpy(priority, "VERBOSE");
			break;
		case WARNING :
			strcpy(priority, "WARNING");
			break;
		case FAILURE :
			strcpy(priority, "FAILURE");
			break;
		case -1:
		{
			flog->_log_err = 1;
			res = NULL;
			return;
		}
	}
	sprintf( res, "%d/%d/%d\t%d:%d:%d.%d\t%s\t%s\t%s\t%s",
	rlog->mDate._day, rlog->mDate._month, rlog->mDate._years,
	rlog->mTime._hours, rlog->mTime._min, rlog->mTime._sec, rlog->mTime._millis,
	priority, rlog->proc_name, rlog->tag, rlog->content);
	flog->_log_err = 0;
	return;
}
size_t countLogs(LOG* flog){
	fseek(flog->rdL, 0, 0);
	size_t x = 0;
	int chr;
	while((chr=getc(flog->rdL))!=-1)
	    if(chr == _BUKA_)
	        x++;
	fseek(flog->rdL, 0, 0);
	return x;
}
void closel(LOG* flog){
	if(flog->rdL == NULL)return;
	fclose(flog->rdL);
	flog->rdL = NULL;
	// freeing or releasing the memory caused by malloc
	free(flog);
}
void clearLogs(LOG* flog){
	int len1 = strlen(flog->_logFile);
	int len2 = strlen(flog->proc);
	
	char logF[len1], proc[len2];
	strcpy(logF, flog->_logFile);
	strcpy(proc, flog->proc);
	closel(flog);
	remove(logF);
	flog = NULL;
	flog = openl(logF, proc);
	flog->_log_err = 0;
}
