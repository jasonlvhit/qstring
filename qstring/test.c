#include "qstring.h"

int main(void){
	qstring s = qstrnew("a short str");
	qstrtoupper(s);
	qprintf(s);
	qstrtolower(s);
	qprintf(s);
	qstrfree(s);
	getchar();
	return 0;
}