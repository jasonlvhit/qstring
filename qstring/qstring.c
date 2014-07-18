#include "qstring.h"
#include <cassert>

/*construct a qstring from const char* (char*) 
 *return a qstring, which equal to the type of char*;
 */ 
qstring qstrnew(const char *cstr_){
	QString *s;
	size_t len_ = strlen(cstr_);
	s = (QString*)malloc(sizeof(QString) + len_ + 1);
	memcpy(s->cstr, cstr_, len_);
	s->cstr[len_] = '\0';
	/*init*/
	s->free = 0;
	s->len = len_;
	s->type = 0;
	s->q_hash = qstrhash(s ->cstr);
	s->ref_count = 1;

	if (s->len < INTERNING_SIZE)
		return qstrintern(s);
	return s->cstr;
}
/*interning a string into the string pool
 */
qstring qstrintern(QString *s){
	if (pool->count >= POOLING_SIZE) // if pool is full
		return s->cstr;

	if (pool->node[s->q_hash]){ //if already in the pool
		QString* temp = pool->node[s->q_hash];
		temp->ref_count++;
		qstrfree(s);
		return temp->cstr;
	}
	//else intern it in 
	pool->node[s->q_hash] = s;
	(pool->count)++;
	s->type = INTERNED;
	return s->cstr;
}

void qstrfree(QString *s){
	assert(s->ref_count == 0);
	free(s);
}

long qstrhash(qstring s){
	return -1;
}

/*return the length of qstring, O(1) tata, haha*/
size_t qstrlen(const qstring s){
	QString *q = (QString*)(s - sizeof(QString));
	return q->len;
}

int main(){
	QString *s;
	s = (QString*)malloc(sizeof(QString) + 8 + 1);
	s->len = 8;
	s->free = 0;
	s->q_hash = -1;
	s->ref_count = 1;
	s->type = 0;
	memcpy(s->cstr, "hello222", 8);
	s->cstr[8] = '\0';
	qstring q = s->cstr;
	int len = qstrlen(q);
	printf("%d", len);
	getchar();
	return 0;
}