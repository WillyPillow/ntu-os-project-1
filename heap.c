#include "heap.h"

static int GetHeight(HeapNode *a) {
	return a ? a->height : 0;
}

HeapNode *Merge(HeapNode *a, HeapNode *b) {
	if (!a || !b) return a ? a : b;
	if (a->key > b->key || (a->key == b->key && a > b)) {
		HeapNode *t = a;
		a = b;
		b = t;
	}
	a->rc = Merge(a->rc, b);
	int lh = GetHeight(a->lc), rh = GetHeight(a->rc);
	if (lh < rh) {
		HeapNode *t = a->lc;
		a->lc = a->rc;
		a->rc = t;
	}
	a->height = (lh < rh ? lh : rh) + 1;
	return a;
}
