/* QString, A simple C dynamic strings library
*
* Copyright (c) 2014 Jason Lyu <jasonlvhit at gmail dot com>
* All rights reserved.
*
* @ https:github.com/jasonlvhit/qstring
* @ jasonlvhit@gmail.com
*/

#include "qstring.h"

/*declare a empty qstring*/
qstring qstrempty(void){
	return NULL;
}

/*construct a qstring from const char* (char*) 
 *return a qstring, which equal to the type of char*;
 */ 
qstring qstrnew(const char *cstr_){
	return _qstrnew(cstr_)->cstr;
}

/* lower level function serve the function for 
 * construct a new qstring,
 * for a given const char* ,
 * return a QString*.
 */
QString* _qstrnew(const char* cstr_){
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
	s->q_hash = qstrhash(s->cstr);
	s->ref_count = 1;
	s->hash_size = -1;

	if (s->len < INTERNING_SIZE)
		return qstrintern(s);
	return s;
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
QString* qstrintern(QString *s){
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

	while ((*temp)!=NULL){
		if (strcmp((*temp)->data->cstr, s->cstr) == 0){
			_qstrfree(s);
			(*temp)->data->ref_count++; //reference count ++
			return (*temp)->data;
		}
		temp = &((*temp)->next);
	}

	*temp = q;
	q->next = NULL;
	q->data->type = INTERNED;

	return q->data;
}

/*expend the size for the interning pool*/
void expand(void){
	if (pool.size == POOLING_SIZE)
		return;

	pool.size *= 2;
	if (pool.size == 0)
		pool.size = POOLING_START_SIZE;
}

/*get the pointer to the interned node q*/
struct qstr_node* _getInternNode(QString* q){
	size_t index = q->q_hash & q->hash_size;
	struct qstr_node* temp = pool.node[index];
	assert(temp);
	while (temp){
		if (strcmp(temp->data->cstr, q->cstr) == 0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}

/*if a qstr is interned into the interning pool, you should free it using this ugly function,
 *this function just reduce the ref_count of the given QString* q by 1,
 *if the given node's ref_count equals to 0, delete it from interning pool
 *
 *!!!
 */
void _internqstrfree(QString* q){
	assert(q->type == INTERNED);
	struct qstr_node* qn = _getInternNode(q);
	if (--(qn->data->ref_count) == 0){/*-- reference_count, if ref_count == 0 , 
									  free the qstr*/
		struct qstr_node *_free = (qn->next);
		free(qn);
		qn = _free; 
		//!!!
	}
}

/*user level function, release a qstr s*/
void qstrfree(qstring s){
	QString* q = (QString*)(s - sizeof(QString));
	if (q->type == INTERNED){/*if interned*/
		//_internqstrfree(q);
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

/*make room for the current long qstr*/
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
		qstring ns = qstrnew(strncat(s, t, t_len));
		return ns;
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

/*tolower*/
void qstrtolower(qstring s){
	QString* q = (QString*)(s - sizeof(QString));
	if (q->type == INTERNED){/*if interned*/
		char* temp = NULL;
		temp = qstrdeepcpy(temp, s, q->len);
		//_internqstrfree(q);/*free the origin interned qstr*/
		for (size_t i = 0; i < q->len; ++i)
			temp[i] = tolower(temp[i]);
		q = _qstrnew(temp);
	}

	for (size_t i = 0; i < q->len; i++)
		s[i] = tolower(s[i]);
}

/*qstrcpy, increase the ref_count by 1*/
qstring qstrcpy(qstring dest, qstring src){
	QString* q_src = (QString*)(src - sizeof(QString));
	q_src->ref_count++;
	dest = src;
	return dest;
}

/*deep copy...whatever the type(interned, or dynamic alloc on heap) of qstr src, 
 *copy it to a new char* dest
 */
char* qstrdeepcpy(char* dest, qstring src, size_t c_len){
	/*if dest point to a memory, free it*/
	if (dest) free(dest);

	dest = (char*)malloc(sizeof(char)*(c_len + 1));
	assert(dest);
	/*todo*/
	if (!dest)
		;
	memcpy(dest, src, c_len + 1);
	return dest;
}

/*to upper*/
void qstrtoupper(qstring s){
	QString* q = (QString*)(s - sizeof(QString));
	if (q->type == INTERNED){/*if interned*/
		char* temp = NULL;
		temp = qstrdeepcpy(temp, s, q->len);
		//_internqstrfree(q);/*free the origin interned qstr*/
		for (size_t i=0; i < q->len; ++i)
			temp[i] = toupper(temp[i]);
		q = _qstrnew(temp);
	}

	for (size_t i = 0; i < q->len; i++)
		s[i] = toupper(s[i]);
}
/*same with standard C lib sprintf function*/
int qsprintf(qstring s, const char* fmt, ...){
	QString *q = (QString*)(s - sizeof(QString));
	va_list ap;
	va_start(ap, fmt);
	qstring temp = qstrformat(fmt, ap);
	if (strlen(temp) >= INTERNING_SIZE && q->type == INTERNED)
		_internqstrfree(q);
	else
		free(q);
	
	s = qstrnew(temp);
	va_end(ap);
	return qstrlen(s);
}

/*print the format qstring, and 
 *return the number of charactor printed.
 */
int qprintf(const char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	qstring s = qstrformat(fmt, ap);
	printf(s);
	return strlen(s);
}

/*return a formated qstring with args list */
qstring qstrformat(const char* fmt, va_list ap){
	char * temp = NULL;
	temp = (char*)malloc(QSTR_FORMAT_TEMP);
	assert(temp);
	//to do : check malloc
	char* result;
	int n = vsnprintf(temp, QSTR_FORMAT_TEMP, fmt, ap);
	if (n >= QSTR_FORMAT_TEMP){
		int sz = QSTR_FORMAT_TEMP * 2;
		for (;;){
			result = malloc(sz);
			//todo: check malloc
			assert(result);
			n = vsnprintf(result, sz, fmt, ap);
			if (n >= sz){
				free(result);
				sz *= 2;
			}
			else{
				break;
			}
		}
	}
	else{
		result = temp;
	}

	QString* q = (QString*)malloc(sizeof(QString) + n + 1);
	//todo : check malloc 
	assert(q);
	q->type = 0;
	q->free = 0;
	q->hash_size = -1;
	q->q_hash = -1;
	q->len = n;
	q->ref_count = 1;
	memcpy(q->cstr, result, n);
	q->cstr[n] = '\0';
	free(result);
	return q->cstr;
}

/*remove space around a qstr*/
qstring qstrtrim(qstring s){
	QString* q = (QString*)(s - sizeof(QString));
	qstring start, end, sp, ep;
	size_t len = 0;
	sp = start = s;
	ep = end = s + qstrlen(s) - 1;
	
	while (sp <= end && strchr(" ", *sp)) sp++;
	while (ep > start && strchr(" ", *ep)) ep--;
	len = (sp - ep)<=0 ? ((ep - sp) + 1) : 0;
	
	if (q->type == INTERNED && len != q ->len){/*interned*/
		char* temp = NULL;
		sp[len] = '\0';
		temp = qstrdeepcpy(temp, sp, len);
		_internqstrfree(q);
		return qstrnew(temp);
	}
	/*else*/
	if (q->cstr != sp) memmove(q->cstr, sp, len);
	q->cstr[len] = '\0';
	q->free += (q->len - len);
	q->len = len;
	return s;
}

/* slice , return a char * of the given range
 * start and end between begin and end;
 * for example, qstrslice("Hello world", 2, 5)
 * will return "llo"
 */
char* qstrslice(qstring s, size_t begin, size_t end){
	assert(end - begin >= 0);
	char* re = NULL;
	re = (char*)malloc(sizeof(char) * (end - begin));
	assert(re);
	// to do: check malloc
	if (re == NULL)
		;
	for (size_t i = 0; i != (end - begin); ++i)
		re[i] = s[begin + i];
	return re;
}

/*release the free space for qstring s
*/
void qstrrelease(qstring s){
	if (qstravail(s) == 0) return;
	QString * q = (QString*)(s - sizeof(QString));
	q = (QString*)realloc(q, sizeof(QString) + qstrlen(s) + 1);
	q->free = 0;
}

/*return the length of qstring, O(1) tata, haha*/
size_t qstrlen(const qstring s){
	return ((QString*)(s - sizeof(QString)))->len;
}

/*return the avail size of current qstring*/
size_t qstravail(const qstring s){
	return ((QString*)(s - sizeof(QString)))->free;
}

QString * _getQString(const qstring s){
	return (QString*)(s - sizeof(QString));
}

