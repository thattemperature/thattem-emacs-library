#ifndef GTOP_DATA_H
#define GTOP_DATA_H

void init_cpu_usage(void);
double get_cpu_usage(void);

double get_mem_usage(void);

double get_swap_usage(void);

void init_net_speed(void);
void get_net_speed(double *in_speed, double *out_speed);

#endif
