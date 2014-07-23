#include "qstring.h"

int main(void){
	qstring s = qstrnew("a short str");
	qstring l = qstrnew("a very long string you have never seen.");
	printf("%d\n%d\n", qstrlen(s), qstrlen(l));
	qstring lc = NULL, sc = NULL;
	lc = qstrcpy(lc, l);
	sc = qstrcpy(sc, s);
	printf("%d\n", qstrcmp(lc, l));
	printf(lc);
	printf("\n");
	printf(sc);
	printf("\n");
	
	l = qstrcat(l, "yes, i have never seen");
	s = qstrcat(s, "a");
	QString* q = (QString*)(s - sizeof(QString));

	printf(l);
	printf("\n");
	printf(sc);
	printf("\n");
	/*
	s = qstrcat(s, "sorry, i am no longer short.");
	q = (QString*)(s - sizeof(QString));
	printf(s);
	printf("\n");
	
	qstrtoupper(s);
	printf(s);
	printf("\n");
	qstrtolower(s);
	printf(s);
	printf("\n");
	*/
	int a = 19;
	//qstring papa = qstrsprintf("helllo %d %s\n", a, s);
	qstring songmingye = qstrnew(" songmingyedahaoren   ");
	songmingye = qstrtrim(songmingye);
	printf(songmingye);
	getchar();
	return 0;
}