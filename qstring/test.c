#include "qstring.h"
#include "assert.h"

int main(void){
	const char *r = "a short str";
	const char *r2 = "A SHORT STR";
	
	qstring s = qstrnew("a short str");
	assert(strcmp(s, r) == 0);
	assert(qstrlen(s) == 11);
	printf("Test Strlen OK\n");

	qstring s2 = qstrnew("a short ");
	assert(qstrlen(s2) == 8);
	qstring s3 = qstrcat(s2, "str");
	assert(qstrlen(s3) == 11);
	assert(s3 == s); // in pool, same as 's'
	qstring s4 = qstrcat(s2, "guy");
	assert(qstrlen(s4) == 14); 
	assert(s4 != s); // not in pool

	qstrtoupper(s);
	assert(strcmp(s, r2) == 0);
	printf("Test ToUpper OK\n");

	qstrtolower(s);
	assert(strcmp(s, r) == 0);
	printf("Test ToLower OK\n");

	qstrfree(s);
	
	printf("All Tests Passed.\n");
	return 0;
}