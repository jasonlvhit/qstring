#include "qstring.h"
#include <assert.h>

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
	s->hash_size = -1;

	if (s->len < INTERNING_SIZE)
		return qstrintern(s);
	return s->cstr;
}

/*user level function, same to the strcmp*/
int qstrcmp(const qstring l, const qstring r){
	return strcmp(l, r);
}

/*lower level function, serve the function qstrequal as below*/
bool _qstrequal(const QString* l, const QString* r){
	/*all the 2 str are INTERNED*/
	if (l->type == INTERNED && r->type == INTERNED)
		return false;
	/*unequal hash value*/
	if (l->q_hash != r->q_hash)
		return false;

	return (strcmp(l->cstr, r->cstr) == 0);
}

/*return true if qstr l equals to the qstr r*/
bool qstrequal(const qstring l, const qstring r){
	if (l == r)
		return true;

	QString *ls = _getQString(l);
	QString *rs = _getQString(r);
	return _qstrequal(ls, rs);
}

/*interning a string into the string pool
 */
qstring qstrintern(QString *s){
	if (pool->count >= pool->size)
		expand();

	size_t index = (s->q_hash & pool->size);
	s->hash_size = pool->size;
	struct qstr_node * q = (struct qstr_node*)malloc(sizeof(struct qstr_node));
	q->data = s;
	//memcpy(q->buffer, s->cstr, (s->len + 1)); //?
	struct qstr_node *temp = pool ->node[index];
	if (!temp)
		(pool->count)++;

	while (temp){
		temp = temp->next;
		if (strcmp(temp->data->cstr, s->cstr) == 0){
			_qstrfree(s);
			temp->data->ref_count++; //reference count ++
			return temp->data->cstr;
		}
	}

	temp = q;
	q->next = NULL;
	q->data->type = INTERNED;
	return q->data->cstr;
}

/*expend the size for the interning pool*/
void expend(){
	if (pool ->size == POOLING_SIZE)
		return 

	pool->size *= 2;
	if (pool->size == 0)
		pool->size = POOLING_START_SIZE;
}

struct qstr_node* _getInternNode(QString* q){
	size_t index = q->q_hash & q->hash_size;
	struct qstr_node* temp = pool->node[index];
	assert(temp);
	while (temp){
		if (strcmp(temp->data->cstr, q->cstr) == 0)
			return temp;
	}
	return NULL;
}

void _internqstrfree(QString* q){
	assert(q->type == INTERNED);
	struct qstr_node* qn = _getInternNode(q);
	if (--(qn->data->ref_count) == 0){
		free(qn->data);
		qn = qn->next; //!!!ÄÚ´æÐ¹Â©
	}
}

/*user level function, release a qstr s*/
void qstrfree(qstring s){
	QString* q = _getQString(s);
	if (q->type == INTERNED){/*if interned*/
		return _internqstrfree(q);
	}
	_qstrfree(q);
}

//lower level function, release the str
static inline void _qstrfree(QString *s){
	free(s);
}

/*SDBM Hash algorithm
*/
unsigned int qstrhash(qstring str){
	unsigned int hash = 0;
	while (str)
		hash = (str++) + (hash << 6) + (hash << 16) - hash;

	return (hash & 0x7FFFFFFF);
}

/*append a char* t to the qstring s, with the length of t*/
qstring qstrncat(qstring s, const char* t, size_t t_len){
	;
}

/*append a char* to the qstr s*/
qstring qstrcat(qstring s, const char* t){
	return qstrncat(s, t, strlen(t));
}

/*return the length of qstring, O(1) tata, haha*/
static inline size_t qstrlen(const qstring s){
	return _getQString(s)->len;
}

/*return the avail size of current qstring*/
static inline size_t qstravail(const qstring s){
	return _getQString(s)->free;
}

static inline QString * _getQString(const qstring s){
	return (QString*)(s - sizeof(QString));
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