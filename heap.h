#pragma once
#include "common.h"

typedef struct HeapNode {
	struct HeapNode *lc, *rc;
	int key, height;
} HeapNode;

HeapNode *Merge(HeapNode *a, HeapNode *b);
