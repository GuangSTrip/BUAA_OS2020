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
void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
	static struct Env *env = NULL;
	/*while (count <= 0 || env == NULL || (env != NULL && env->env_status != ENV_RUNNABLE)) {
		count = 0;
		if (env != NULL) {
			LIST_REMOVE(env, env_sched_link);
			if (env->env_status != ENV_FREE) {
				LIST_INSERT_TAIL(&env_sched_list[1-point], env, env_sched_link);
			}
		}
		if (LIST_EMPTY(&env_sched_list[point])) {
			point = 1 - point;
		}
		if (LIST_EMPTY(&env_sched_list[point])) {
			//panic("^^^^^^^there is no runnable env^^^^^^^by sched_yield");
			continue;
		}
		env = LIST_FIRST(&env_sched_list[point]);
		if (env != NULL) {
			count = env->env_pri;
		}
	}*/
	//----------------------lab3-2-exam
	printf("\n");
	while (count <= 0 || env == NULL || (env != NULL && env->env_status != ENV_RUNNABLE)) {
		count = 0;
		if (env != NULL) {
			LIST_REMOVE(env, env_sched_link);
			if (env->env_status != ENV_FREE) {
				if (env->env_pri % 2 == 1) {
                       			 LIST_INSERT_TAIL(&env_sched_list[(point + 1) % 3], env, env_sched_link);
                			//printf("send to point : %d\n", (point + 1) % 3);
				} else {
	                 	       LIST_INSERT_TAIL(&env_sched_list[(point + 2) % 3], env, env_sched_link);
					//printf("send to point : %d\n", (point + 2) % 3);
                		}
			}
		}
		if (LIST_EMPTY(&env_sched_list[point])) {
			point = (point + 1) % 3;
			env = NULL;
			continue;
		}
		env = LIST_FIRST(&env_sched_list[point]);
		if (env != NULL) {
			count = env->env_pri * (1 << point);
		}
		
	}
	//printf("point is : %d\n",point);
	//printf("count = %d\n", count);
	count--;
	env_run(env);
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
