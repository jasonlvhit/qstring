#ifndef __QSTRING_H
#define __QSTRING_H
#include <stdio.h>
#include <stdarg.h>

#define bool int
#define false 0
#define true 1

#define POOLING_SIZE 1024
#define POOLING_START_SIZE 16
#define INTERNING_SIZE 32
#define INTERNED 1

typedef char* qstring;

typedef struct {
	size_t len;
	size_t free;
	size_t type;
	size_t ref_count;
	size_t hash_size;
	unsigned int q_hash;
	char cstr[];
}QString;

struct qstr_node{
	QString *data;
	QString *next;
	//char buffer[INTERNING_SIZE];
};

typedef struct String_Pool{
	struct qstr_node *node[POOLING_SIZE];
	size_t size; //2 pow, start from 16
	size_t count;
}Pool;

Pool *pool;


/*user level function*/
qstring qstrnew(const char *);
qstring qstrintern(QString*);
static inline size_t qstrlen(const qstring);
static inline size_t qstravail(const qstring);
unsigned int qstrhash(qstring);
void qstrfree(qstring);
qstring qstrncat(const char*);
bool qstrequal(const QString*, const QString*);
int qstrcmp(const qstring, const qstring);
int qstrtrim(const qstring);
qstring qstrcpy(qstring, qstring);
qstring qstrncpy(qstring, qstring, size_t);

/*lower level function*/
void _internqstrfree(QString*);
static inline void _qstrfree(QString*);
bool _qstrequal(const qstring, const qstring);


#endif