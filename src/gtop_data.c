#include <glib.h>
#include <glibtop.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>
#include <glibtop/swap.h>

static glibtop_cpu old_cpu, new_cpu;
void init_cpu_usage() { glibtop_get_cpu(&old_cpu); }
double get_cpu_usage() {
  glibtop_get_cpu(&new_cpu);
  guint64 total_delta = new_cpu.total - old_cpu.total;
  guint64 user_delta = new_cpu.user - old_cpu.user;
  guint64 sys_delta = new_cpu.sys - old_cpu.sys;
  guint64 nice_delta = new_cpu.nice - old_cpu.nice;
  double usage =
      (double)(user_delta + sys_delta + nice_delta) / (double)total_delta;
  old_cpu = new_cpu;
  return usage;
}

static glibtop_mem now_mem;
double get_mem_usage() {
  glibtop_get_mem(&now_mem);
  double usage = (double)now_mem.user / (double)now_mem.total;
  return usage;
}

static glibtop_swap now_swap;
double get_swap_usage() {
  glibtop_get_swap(&now_swap);
  double usage = (double)now_swap.used / (double)now_swap.total;
  return usage;
}
