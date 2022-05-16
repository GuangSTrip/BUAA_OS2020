#include "sched.h"
void FCFS (
    int number_of_jobs,
    const int job_submitted_time [],
    const int job_required_time [],
    int job_sched_start []
) {
	int i = 0;
	int time = 0;
	int tail = -1;
	while (i < number_of_jobs) {
		while (tail + 1 < number_of_jobs && job_submitted_time[tail + 1] <= time) {
			tail++;
		}
		if (tail < i) {
			time = job_submitted_time[i];
			while (tail + 1 < number_of_jobs && job_submitted_time[tail + 1] <= time) {
				tail++;
			}
		}
		job_sched_start[i] = time;
		time += job_required_time[i];
		i++;
	}
}


void SJF (
    int number_of_jobs,
    const int job_submitted_time [],
    const int job_required_time [],
    int job_sched_start []
) {
	int i = 0;
	int time = 0;
	int tail = -1;
	int find[2001];
	for (i = 0;i < number_of_jobs;i++) {
		find[i] = 0;
	}
	i = 0;
	while (i < number_of_jobs) {
		while (tail + 1 < number_of_jobs && job_submitted_time[tail + 1] <= time) {
			tail++;
		}
		if (tail < i) {
			time = job_submitted_time[i];
			while (tail + 1 < number_of_jobs && job_submitted_time[tail + 1] <= time) {
				tail++;
			}
		}
		int j = -1;
		int k;
		for (k = 0; k <= tail; k++) {
			if (find[k] == 0) {
				if (j == -1) {
					j = k;
				} else if (job_required_time[k] < job_required_time[j]) {
					j = k;
				}
			}
		}
		find[j] = 1;
		job_sched_start[j] = time;
		time += job_required_time[j];
		i++;
	}
}
