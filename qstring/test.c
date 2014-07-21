#include "qstring.h"

int main(){
	qstring s = qstrnew("a short str");
	qstring l = qstrnew("a very long string you have never seen.");
	printf("%d\n%d\n", qstrlen(s), qstrlen(l));
	qstring lc = NULL, sc = NULL;
	lc = qstrcpy(lc, l, qstrlen(l));
	sc = qstrcpy(sc, s, qstrlen(s));
	printf("%d\n", qstrcmp(lc, l));
	printf(lc);
	printf("\n");
	printf(sc);
	printf("\n");
	l = qstrcat(l, "yes, i have never seen");
	s = qstrcat(s, "a");
	printf(lc);
	printf("\n");
	printf(sc);
	printf("\n");
	s = qstrcat(s, "sorry, i am no longer short.");
	printf(s);
	printf("\n");
	qstrtoupper(s);
	printf(s);
	printf("\n");
	qstrtolower(s);
	printf(s);
	getchar();
	return 0;
}