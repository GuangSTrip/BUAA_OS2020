#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.15 ***/
extern struct Env_list env_sched_list[];
extern struct Env* curenv;
extern struct Pth* curpth;
extern struct Pth_list pth_sched_list[];
void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
	static struct Env *env = NULL;
	static struct Pth *pth = NULL;
	//printf("1\n");
	while (count <= 0 || pth  == NULL || (pth != NULL && pth->pth_status != ENV_RUNNABLE)) {
		count = 0;
		//printf("11\n");
		if (pth != NULL) {
			//printf("121\n");
			LIST_REMOVE(pth, pth_sched_link);
			//printf("131\n");
			if (pth->pth_status != ENV_FREE) {
				//printf("1311\n");
				LIST_INSERT_TAIL(&pth_sched_list[1 - point], pth, pth_sched_link);
				//printf("1312\n");
			}
			//printf("141\n");
		}
		//printf("22\n");
		if (LIST_EMPTY(&pth_sched_list[point])) {
			point = 1 - point;
		}
		//printf("33\n");
		if (LIST_EMPTY(&pth_sched_list[point])) {
			//panic("^^^^^^^there is no runnable env^^^^^^^by sched_yield");
			continue;
		}
		//printf("44\n");
		pth = LIST_FIRST(&pth_sched_list[point]);
		if (pth != NULL) {
			count = pth->pth_pri;
		}
		//printf("55\n");
	}
	count--;
	//printf("sched to %d\n", pth->pth_id);
	pth_run(pth);
	//env_run(LIST_FIRST(env_sched_list));
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
}
