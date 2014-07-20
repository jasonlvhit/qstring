#ifndef __QSTRING_H
#define __QSTRING_H
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

#define bool int
#define false 0
#define true 1

#define POOLING_SIZE 1024
#define POOLING_START_SIZE 16
#define INTERNING_SIZE 16
#define INTERNED 1

#define QSTR_MAX_PREALLOC (1024*1024)

typedef char* qstring;

typedef struct {
	size_t len;
	size_t free;
	size_t type;
	size_t ref_count;
	int hash_size;
	unsigned int q_hash;
	char cstr[];
}QString;

struct qstr_node{
	QString *data;
	struct qstr_node *next;
	//char buffer[INTERNING_SIZE];
};

typedef struct String_Pool{
	struct qstr_node *node[POOLING_SIZE];
	size_t size; //2 pow, start from 16
	size_t count;
}Pool;

Pool pool;

/*user level function*/
qstring qstrnew(const char *);
qstring qstrintern(QString*);
size_t qstrlen(const qstring);
size_t qstravail(const qstring);
unsigned int qstrhash(qstring);
void qstrfree(qstring);
qstring qstrcat(qstring, const char*);
qstring qstrncat(qstring, const char*, size_t);
bool qstrequal(const qstring, const qstring);
int qstrcmp(const qstring, const qstring);
int qstrtrim(const qstring);
qstring qstrcpy(qstring, qstring);
qstring qstrncpy(qstring, qstring, size_t);

/*lower level function*/
void expand(void);
void _internqstrfree(QString*);
void _qstrfree(QString*);
bool _qstrequal(const QString*, const QString*);
QString* qstrMakeRoom(QString*, size_t);

struct qstr_node* _getInternNode(QString*);


#endif