#include "qstring.h"

int main(){
	qstring a = qstrnew("intern");
	printf("%d", qstrlen(a));

	a = qstrcat(a, "jason");
	printf(a);
	printf("%d", qstrlen(a));
	QString* q = (QString*)(a - sizeof(QString));
	getchar();
	return 0;
}