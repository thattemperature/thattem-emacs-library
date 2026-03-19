#include <glib.h>
#include <glibtop.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>
#include <glibtop/netlist.h>
#include <glibtop/netload.h>
#include <glibtop/swap.h>

static glibtop_cpu old_cpu, new_cpu;
void init_cpu_usage() { glibtop_get_cpu(&old_cpu); }
double get_cpu_usage() {
  glibtop_get_cpu(&new_cpu);
  guint64 total_delta = new_cpu.total - old_cpu.total;
  guint64 user_delta = new_cpu.user - old_cpu.user;
  guint64 sys_delta = new_cpu.sys - old_cpu.sys;
  guint64 nice_delta = new_cpu.nice - old_cpu.nice;
  double usage = (double)(user_delta + sys_delta + nice_delta) /
                 (double)(total_delta == 0 ? 1 : total_delta);
  old_cpu = new_cpu;
  return usage;
}

static glibtop_mem now_mem;
double get_mem_usage() {
  glibtop_get_mem(&now_mem);
  double usage =
      (double)now_mem.user / (double)(now_mem.total == 0 ? 1 : now_mem.total);
  return usage;
}

static glibtop_swap now_swap;
double get_swap_usage() {
  glibtop_get_swap(&now_swap);
  double usage = (double)now_swap.used /
                 (double)(now_swap.total == 0 ? 1 : now_swap.total);
  return usage;
}

static const guint64 loopback_mask = 1UL << GLIBTOP_IF_FLAGS_LOOPBACK;
static gchar **interface_names;
static glibtop_netlist netlist;
static glibtop_netload netload;
static guint64 old_in, old_out;
static guint64 new_in, new_out;
static gint64 old_time, new_time;
static void get_net_load(guint64 *in, guint64 *out) {
  interface_names = glibtop_get_netlist(&netlist);
  guint64 total_in = 0UL, total_out = 0UL;
  for (guint64 i = 0UL; i < netlist.number; i++) {
    glibtop_get_netload(&netload, interface_names[i]);
    if (netload.if_flags & loopback_mask)
      continue;
    total_in += netload.bytes_in;
    total_out += netload.bytes_out;
  }
  g_strfreev(interface_names);
  *in = total_in;
  *out = total_out;
}
void init_net_speed() {
  old_time = g_get_monotonic_time();
  get_net_load(&old_in, &old_out);
}
void get_net_speed(double *in_speed, double *out_speed) {
  new_time = g_get_monotonic_time();
  get_net_load(&new_in, &new_out);
  guint64 in_delta = new_in > old_in ? new_in - old_in : 0;
  guint64 out_delta = new_out > old_out ? new_out - old_out : 0;
  double time_delta = (double)(new_time == old_time ? 1 : new_time - old_time) /
                      (double)G_USEC_PER_SEC;
  *in_speed = (double)in_delta / time_delta;
  *out_speed = (double)out_delta / time_delta;
  old_time = new_time;
  old_in = new_in;
  old_out = new_out;
}
