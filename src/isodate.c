#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include "isodate.h"

const char* formats[] = {"%Y",
						"%Y-%m",
						"%Y-%m-%d",
						"%m-%d",
						"%H:%M",
						"%H:%M:%S",
						"%Y-%m-%dT%H:%M",
						"%Y-%m-%d%t%H:%M",
						"%Y-%m-%d%t%H:%M:%S",
						"%Y-%m-%dT%H:%M:%S",
						NULL};

char validateISOTime(uint8_t *data, size_t size) {
	struct tm tm;
	char* tstr = malloc(size + 1);
	const char* f;
	int i;
	
	memcpy(tstr, data, size);
	tstr[size] = 0;
	assert(strlen(tstr) == size);
	
	f = formats[0];
	for(i=1;f;i++) {
		if(strptime(tstr, f, &tm) == tstr+size) {
			return 1;
		}
		f = formats[i];
	}
	return 0;
}

void test(char* what, char expected) {
	printf("Testing \"%s\" with an expected ", what);
	if(expected) {
		printf("success....");
	} else {
		printf("failure....");
	}
	assert(validateISOTime((uint8_t*)what, strlen(what)) == expected);
	printf("test passed\n");
}

void tester(void) {
	test("2011", 1);
	test("2011-11", 1);
	test("2011-11-12", 1);
	test("11-12", 1);
	test("14:54", 1);
	test("14:54:39", 1);
	test("2011-11-12 14:54", 1);
	test("2011-11-12 14:54:39", 1);
	test("2011-11-12T14:54", 1);
	test("2011-11-12T14:54:39", 1);
	/*test("14:54:39+0800", 1);
	test("14:54:39-0800", 1);
	test("14:54-0800", 1);
	test("2011-11-12T14:54:39-0800", 1);
	test("2011-11-12T14:54:39+0800", 1);
	test("2011-11-12T14:54+0800", 1);
	test("2011-11-12T14:54-0800", 1);
	test("2011-11-12 14:54:39-0800", 1);
	test("2011-11-12 14:54:39+0800", 1);
	test("2011-11-12 14:54+0800", 1);
	test("2011-11-12 14:54-0800", 1);*/
	test("some string", 0);
	test("2011-11-12 14:54+0800 asdf", 0);
	test("", 0);
	test("2011-11-12 +0800", 0);
	//test("10-10-10", 0);
	test("10-2010", 0);
	test("2010-10", 1);
	test("2010-10-10-10", 0);
	printf("All tests passed\n");
}

int main(void) {
	tester();
	return 1;
}
