#include <time.h>
#include <stdio.h>

struct carp_thrd_context {
	Lambda *lambda;
	void *arg;
};

int carp_thrd_fn_run(void *context) {
	puts("Running from thread");
	fflush(stdout);
	struct carp_thrd_context *cxt = (struct carp_thrd_context *)context;
	int ret_val = ((int(*)(void*,void*))(cxt->lambda->callback))(cxt->lambda->env, cxt->arg);
	free(cxt);
	return ret_val;
}

thrd_t carp_thrd_fn_create(Lambda *fn, void *arg, int *result) {
	thrd_t thread;
	struct carp_thrd_context *cxt = malloc(sizeof(struct carp_thrd_context));
	cxt->lambda = fn;
	cxt->arg = arg;
	puts("Starting thread!");
	fflush(stdout);
	*result = thrd_create(&thread,carp_thrd_fn_run, cxt);
	return thread;
}

thrd_t carp_thrd_create(thrd_start_t fn , void *arg, int *result) {
	thrd_t thread;
	*result = thrd_create(&thread, fn, arg);
	return thread;
}

int carp_thrd_sleep(Long seconds, Long nano_seconds) {
	return thrd_sleep(&(struct timespec){.tv_sec=seconds, .tv_nsec=nano_seconds}, NULL);
}
