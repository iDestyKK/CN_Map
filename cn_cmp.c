/*
 * CN Comparison Functions
 * 
 * Description:
 *     This supplies functions to compare data types. This is used mainly for
 *     CN Data structures that require sorting such as CN_Maps. The functions
 *     all have similar return values. -1 if arg0 is less than arg1, 0 if arg0
 *     is equal to arg1, and 1 if arg0 is greater than arg1. The only
 *     exception is cn_cmp_cstr, which returns "how different" the first
 *     string is compared to the second.
 * 
 * Author:
 *     Clara Van Nguyen
 */

#include "cn_cmp.h"

//C-String Comparison
CNC_COMP cn_cmp_cstr(void* arg0, void* arg1) {
	char* a = *(char**)arg0,
	    * b = *(char**)arg1;
	
	//return strcmp(a, b) < 0 ? CN_CMP_LESS : CN_CMP_GREATER;
	return strcmp(a, b);
}

//Signed Reals
//Char Comparison
CNC_COMP cn_cmp_char(void* arg0, void* arg1) {
	char* a = (char *)arg0,
	    * b = (char *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}


//Integer Comparison
CNC_COMP cn_cmp_int(void* arg0, void* arg1) {
	int* a = (int *)arg0,
	   * b = (int *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}

//Short Comparison
CNC_COMP cn_cmp_short(void* arg0, void* arg1) {
	short* a = (short *)arg0,
	     * b = (short *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}

//Long Comparison
CNC_COMP cn_cmp_long(void* arg0, void* arg1) {
	long* a = (long *)arg0,
	    * b = (long *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}

//Long Long Comparison
CNC_COMP cn_cmp_ll(void* arg0, void* arg1) {
	long long* a = (long long *)arg0,
	         * b = (long long *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}

//Float Comparison
CNC_COMP cn_cmp_float(void* arg0, void* arg1) {
	float* a = (float *)arg0,
         * b = (float *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}


//Double Comparison
CNC_COMP cn_cmp_double(void* arg0, void* arg1) {
	double* a = (double *)arg0,
	      * b = (double *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}


//Long Double Comparison
CNC_COMP cn_cmp_ldouble(void* arg0, void* arg1) {
	long double* a = (long double *)arg0,
	           * b = (long double *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}

//Unsigned Reals
//Unsigned Char Comparison
CNC_COMP cn_cmp_uchar(void* arg0, void* arg1) {
	unsigned char* a = (unsigned char *)arg0,
	             * b = (unsigned char *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}


//Unsigned Integer Comparison
CNC_COMP cn_cmp_uint(void* arg0, void* arg1) {
	unsigned int* a = (unsigned int *)arg0,
	            * b = (unsigned int *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}

//Unsigned Short Comparison
CNC_COMP cn_cmp_ushort(void* arg0, void* arg1) {
	unsigned short* a = (unsigned short *)arg0,
	              * b = (unsigned short *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}

//Unsigned Long Comparison
CNC_COMP cn_cmp_ulong(void* arg0, void* arg1) {
	unsigned long* a = (unsigned long *)arg0,
	             * b = (unsigned long *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}

//Unsigned Long Long Comparison
CNC_COMP cn_cmp_ull(void* arg0, void* arg1) {
	unsigned long long* a = (unsigned long long *)arg0,
	                  * b = (unsigned long long *)arg1;

	return (*a < *b) ? CN_CMP_LESS : (*a > *b) ? CN_CMP_GREATER : CN_CMP_EQUAL;
}
