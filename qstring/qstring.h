#ifndef __QSTRING_H
#define __QSTRING_H
#include <stdio.h>

#define POOLING_SIZE 1024
#define INTERNING_SIZE 32
#define INTERNED 1

typedef char* qstring;

typedef struct {
	size_t len;
	size_t free;
	size_t type;
	size_t ref_count;
	long q_hash;
	char cstr[];
}QString;

typedef struct String_Pool{
	size_t count;
	QString *node[POOLING_SIZE];
}Pool[1];

Pool pool;

qstring qstrintern(QString*);
qstring qstrnew(const char *);
size_t qstrlen(const qstring);
long qstrhash(const qstring);
void qstrfree(QString*);



#endif