#include "heap.h"
#include "policy.h"
#include "proc.h"
#include "timeutils.h"

void PolicyFifo() {
	init_time = GetTimeNs();
	for (int i = 0; i < n; i++) {
		WaitUntil(ready_time[id_by_ready[i]]);
		int pid = fork();
		if (pid == 0) {
			IncPriority(pid, kCpuChild);
			RunChildProc(id_by_ready[i]);
		} else {
			IncPriority(pid, kCpuChild);
			assert(wait(NULL) > 0);
		}
	}
}

void PolicyRr() {
	int *prv = (int *)malloc((n + 1) * sizeof(int)),
	    *nxt = (int *)malloc((n + 1) * sizeof(int)),
	    *pid = (int *)malloc(n * sizeof(int));
	memset(prv, -1, (n + 1) * sizeof(int));
	memset(nxt, -1, (n + 1) * sizeof(int));
	memset(pid, 0, n * sizeof(int));
	prv[n] = nxt[n] = n;
	bool *dead = (bool *)malloc((n + 1) * sizeof(bool)),
	     *rem = (bool *)malloc((n + 1) * sizeof(bool));
	memset(dead, 0, (n + 1) * sizeof(bool));
	memset(rem, 0, (n + 1) * sizeof(bool));
	dead[n] = rem[n] = true;
	int cur = n, last_t = 0;
	init_time = GetTimeNs();
	for (int i = 0, fin = 0; fin < n;) {
		while (fin < n) {
			int id = id_by_end[fin];
			if (dead[id] || (pid[id] && waitpid(pid[id], NULL, WNOHANG) == pid[id])) {
				if (!rem[id]) {
					prv[nxt[id]] = prv[id];
					nxt[prv[id]] = nxt[id];
					dead[id] = rem[id] = true;
				}
				++fin;
			} else {
				break;
			}
		}
		while (i < n && ready_time[id_by_ready[i]] <= GetTime() + kEps) {
			WaitUntil(ready_time[id_by_ready[i]]);
			int t_pid = fork();
			if (t_pid == 0) {
				//if (nxt[n] != n) DecPriority(0, kCpuChild);
				//else IncPriority(0, kCpuChild);
				RunChildProc(id_by_ready[i]);
			}
			if (nxt[n] == n) {
				IncPriority(t_pid, kCpuChild);
				last_t = GetTime();
				cur = i;
			} else {
				DecPriority(t_pid, kCpuChild);
			}
			pid[id_by_ready[i]] = t_pid;
			prv[id_by_ready[i]] = prv[n];
			nxt[id_by_ready[i]] = n;
			nxt[prv[n]] = id_by_ready[i];
			prv[n] = id_by_ready[i];
			++i;
		}
		int t;
		if (nxt[n] != n && ((t = GetTime()) - last_t >= kTimeQuantumUnits || dead[cur])) {
			// Try to find next process that is still alive
			int last = cur;
			while (nxt[n] != n) {
				cur = nxt[cur] == n ? nxt[n] : nxt[cur];
				if (!dead[cur] && waitpid(pid[cur], NULL, WNOHANG) != pid[cur]) {
					if (!dead[last]) {
						DecPriority(pid[last], -1);
						if ((t - last_t) + kEps >= execution_time[last]) {
							assert(waitpid(pid[last], NULL, 0) == pid[last]);
							dead[last] = true;
						}
					}
					IncPriority(pid[cur], -1);
					t = GetTime();
					execution_time[last] -= t - last_t;
					last_t = t;
					break;
				} else {
					dead[cur] = true;
				}
			}
		}
	}
}

void PolicySjf() {
	HeapNode *heap = (HeapNode *)malloc(n * sizeof(HeapNode)), *root = NULL;
	memset(heap, 0, n * sizeof(HeapNode));
	for (int i = 0; i < n; i++) {
		heap[i].key = execution_time[i];
	}
	int cur = 0;
	while (cur < n &&
	       ready_time[id_by_ready[cur]] <= ready_time[id_by_ready[0]]) {
		root = Merge(root, &heap[id_by_ready[cur++]]);
	}
	init_time = GetTimeNs();
	WaitUntil(ready_time[id_by_ready[0]]);
	while (root) {
		int id = (int)(root - heap), pid;
		if ((pid = fork()) == 0) {
			IncPriority(0, kCpuChild);
			RunChildProc(id);
		}
		IncPriority(pid, kCpuChild);
		root = Merge(root->lc, root->rc);
		bool alive = true;
		do {
			while (cur < n && ready_time[id_by_ready[cur]] <= GetTime() + kEps) {
				WaitUntil(ready_time[id_by_ready[cur]]);
				root = Merge(root, &heap[id_by_ready[cur++]]);
			}
		} while ((alive &= waitpid(pid, NULL, WNOHANG) != pid) || (cur < n && !root));
	}
}

void PolicyPsjf() {
	HeapNode *heap = (HeapNode *)malloc(n * sizeof(HeapNode)), *root = NULL;
	memset(heap, 0, n * sizeof(HeapNode));
	int *pid = (int *)malloc(n * sizeof(int));
	memset(pid, 0, n * sizeof(int));
	for (int i = 0; i < n; i++) {
		heap[i].key = execution_time[i];
	}
	int cur = 0;
	while (cur < n &&
	       ready_time[id_by_ready[cur]] <= ready_time[id_by_ready[0]]) {
		root = Merge(root, &heap[id_by_ready[cur++]]);
	}
	init_time = GetTimeNs();
	WaitUntil(ready_time[id_by_ready[0]]);
	while (root) {
		int id = (int)(root - heap), cur_proc_start = GetTime();
		bool alive = true;
		if (pid[id] == 0) {
			if ((pid[id] = fork()) == 0) {
				IncPriority(0, kCpuChild);
				RunChildProc(id);
			}
			IncPriority(pid[id], kCpuChild);
		} else {
			IncPriority(pid[id], -1);
		}
		root = Merge(root->lc, root->rc);
		do {
			while (cur < n && ready_time[id_by_ready[cur]] <= GetTime() + kEps) {
				WaitUntil(ready_time[id_by_ready[cur]]);
				int t = GetTime();
				root = Merge(root, &heap[id_by_ready[cur]]);
				if (alive && heap[id_by_ready[cur]].key < heap[id].key - (t - cur_proc_start)) {
					if (t - cur_proc_start + kEps >= heap[id].key) {
						assert(waitpid(pid[id], NULL, 0) == pid[id]);
						alive = false;
						++cur;
					} else {
						// Preempt!
						heap[id].key -= t - cur_proc_start;
						heap[id].lc = heap[id].rc = NULL;
						root = Merge(root, &heap[id]);
						DecPriority(pid[id], -1);
						++cur;
						goto NextProc;
					}
				} else {
					++cur;
				}
			}
		} while ((alive &= waitpid(pid[id], NULL, WNOHANG) != pid[id]) || (cur < n && !root));
NextProc:;
	}
}
