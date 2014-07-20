#include "qstring.h"
#include <assert.h>

/*construct a qstring from const char* (char*) 
 *return a qstring, which equal to the type of char*;
 */ 
qstring qstrnew(const char *cstr_){
	QString *s;
	size_t len_ = strlen(cstr_);
	s = (QString*)malloc(sizeof(QString) + len_ + 1);
	/*todo*/
	if (s == NULL)
		;
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

	return (!strcmp(l->cstr, r->cstr));
}

/*return true if qstr l equals to the qstr r*/
bool qstrequal(const qstring l, const qstring r){
	if (l == r)
		return true;

	QString *ls = (QString*)(l - sizeof(QString));
	QString *rs = (QString*)(r - sizeof(QString));
	return _qstrequal(ls, rs);
}

/*interning a string into the string pool
 */
qstring qstrintern(QString *s){
	if (pool.count >= pool.size)
		expand();

	size_t index = (s->q_hash & (pool.size - 1));
	s->hash_size = pool.size - 1;
	struct qstr_node * q = (struct qstr_node*)malloc(sizeof(struct qstr_node));
	/*todo*/
	if (q == NULL)
		;
	q->data = s;
	//memcpy(q->buffer, s->cstr, (s->len + 1)); //?
	struct qstr_node **temp = &pool.node[index];
	if (!(*temp))
		(pool.count)++;

	while ((*temp)){
		temp = &((*temp)->next);
		if (strcmp((*temp)->data->cstr, s->cstr) == 0){
			_qstrfree(s);
			(*temp)->data->ref_count++; //reference count ++
			return (*temp)->data->cstr;
		}
	}

	*temp = q;
	q->next = NULL;
	q->data->type = INTERNED;
	return q->data->cstr;
}

/*expend the size for the interning pool*/
void expand(void){
	if (pool.size == POOLING_SIZE)
		return;

	pool.size *= 2;
	if (pool.size == 0)
		pool.size = POOLING_START_SIZE;
}

struct qstr_node* _getInternNode(QString* q){
	size_t index = q->q_hash & q->hash_size;
	struct qstr_node* temp = pool.node[index];
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
	if (--(qn->data->ref_count) == 0){/*-- reference_count, if ref_count == 0 , 
									  free the qstr*/
		//free(qn->data);
		qn = qn->next; //!!!ÄÚ´æÐ¹Â©
	}
}

/*user level function, release a qstr s*/
void qstrfree(qstring s){
	QString* q = (QString*)(s - sizeof(QString));
	if (q->type == INTERNED){/*if interned*/
		_internqstrfree(q);
		return;
	}
	_qstrfree(q);
}

//lower level function, release the str
void _qstrfree(QString *s){
	free(s);
}

/*SDBM Hash algorithm
*/
unsigned int qstrhash(qstring str){
	unsigned int hash = 0;
	while (*str!='\0')
		hash = (*str++) + (hash << 6) + (hash << 16) - hash;

	return (hash & 0x7FFFFFFF);
}

QString* qstrMakeRoom(QString* q, size_t addlen){
	if (addlen < q->free)
		return q;
	size_t newlen = q->len + addlen;
	if (newlen < QSTR_MAX_PREALLOC)
		newlen *= 2;
	else
		newlen += QSTR_MAX_PREALLOC;

	/*realloc*/
	q = realloc(q, sizeof(QString) + newlen + 1);
	/*todo*/
	if (q == NULL)
		;
	q->free = newlen - q->len;
	return q;
}

/*append a char* t to the qstring s, with the length of t */
qstring qstrncat(qstring s, const char* t, size_t t_len){
	QString * q = ((QString*)(s - sizeof(QString)));
	if (q->type == INTERNED){/*if interned*/
		s = qstrnew(strncat(s, t, t_len));
		_internqstrfree(q);
		return s;
	}
	/*realloc*/
	q = qstrMakeRoom(q, t_len);
	assert(q);
	if (q == NULL)
		;
	s = q->cstr;
	memcpy(s + q->len, t, t_len);
	q->len += t_len;
	q->free -= t_len;
	q->cstr[q->len] = '\0';

	return q->cstr;
}

/*append a char* to the qstr s */
qstring qstrcat(qstring s, const char* t){
	return qstrncat(s, t, strlen(t));
}


/*return the length of qstring, O(1) tata, haha*/
size_t qstrlen(const qstring s){
	return ((QString*)(s - sizeof(QString)))->len;
}

/*return the avail size of current qstring*/
size_t qstravail(const qstring s){
	return ((QString*)(s - sizeof(QString)))->free;
}

/*
QString * _getQString(const qstring s){
	return (QString*)(s - sizeof(QString));
}
*/

