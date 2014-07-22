#ifndef __QSTRING_H
#define __QSTRING_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#define bool char
#define false 0
#define true 1
/*Interning pool*/
#define POOLING_SIZE 1024
#define POOLING_START_SIZE 16
#define INTERNING_SIZE 16
#define INTERNED 1
/*for dynamic string append*/
#define QSTR_MAX_PREALLOC (1024*1024)

typedef char* qstring;

typedef struct {
	size_t len;
	size_t free;	
	size_t type;	//normal or interned
	size_t ref_count;
	int hash_size;  //2pow - 1
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

//const char* cset = "\n\t\r "; //space charactor set

/*user level function*/
qstring qstrempty(void);
qstring qstrnew(const char *);
size_t qstrlen(const qstring);
size_t qstravail(const qstring);
unsigned int qstrhash(qstring);
void qstrfree(qstring);
qstring qstrcat(qstring, const char*);
qstring qstrncat(qstring, const char*, size_t);
bool qstrequal(const qstring, const qstring);
int qstrcmp(const qstring, const qstring);
qstring qstrcpy(qstring, qstring);
qstring qstrncpy(qstring, qstring, size_t);
void qstrtolower(qstring);
void qstrtoupper(qstring);
qstring qstrcpy(qstring dest, qstring src);
char* qstrdeepcpy(char* dest, qstring src, size_t);
size_t qprintf(const char*, ...);
qstring qsprintf(qstring, const char* fmt, ...);
qstring qstrtrim(qstring);


/*lower level function*/
void expand(void);
void _internqstrfree(QString*);
void _qstrfree(QString*);
bool _qstrequal(const QString*, const QString*);
QString* qstrMakeRoom(QString*, size_t);
qstring qstrintern(QString*);
struct qstr_node* _getInternNode(QString*);
qstring qstrformat(const char*, va_list);

#endif /*__QSTRING_H*/