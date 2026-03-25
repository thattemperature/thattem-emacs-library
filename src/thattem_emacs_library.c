#include <emacs-module.h>
#include <gtop_data.h>

int plugin_is_GPL_compatible;

static emacs_value cpu_usage(emacs_env *env, ptrdiff_t nargs,
                             emacs_value args[], void *data) {
  return env->make_float(env, get_cpu_usage());
}
static void register_cpu_usage(emacs_env *env) {
  init_cpu_usage();

  emacs_value func = env->make_function(
      env, 0, 0, cpu_usage,
      "Return CPU usage as a floating point number.\n"
      "\n"
      "The usage is calculated by this formula:\n"
      " (user time + system time + nice time) / total_time\n"
      "Start timing from the previous call of this function.",
      NULL);
  emacs_value symbol = env->intern(env, "thattem-cpu-usage");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

static emacs_value mem_usage(emacs_env *env, ptrdiff_t nargs,
                             emacs_value args[], void *data) {
  return env->make_float(env, get_mem_usage());
}
static void register_mem_usage(emacs_env *env) {
  emacs_value func =
      env->make_function(env, 0, 0, mem_usage,
                         "Return memory usage as a floating point number.\n"
                         "\n"
                         "The usage is calculated by this formula:\n"
                         "used memory / total memory",
                         NULL);
  emacs_value symbol = env->intern(env, "thattem-mem-usage");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

static emacs_value swap_usage(emacs_env *env, ptrdiff_t nargs,
                              emacs_value args[], void *data) {
  return env->make_float(env, get_swap_usage());
}
static void register_swap_usage(emacs_env *env) {
  emacs_value func =
      env->make_function(env, 0, 0, swap_usage,
                         "Return swap usage as a floating point number.\n"
                         "\n"
                         "The usage is calculated by this formula:\n"
                         "used swap / total swap",
                         NULL);
  emacs_value symbol = env->intern(env, "thattem-swap-usage");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

static emacs_value net_speed(emacs_env *env, ptrdiff_t nargs,
                             emacs_value args[], void *data) {
  double in, out;
  get_net_speed(&in, &out);
  emacs_value in_value = env->make_float(env, in);
  emacs_value out_value = env->make_float(env, out);
  emacs_value values[] = {in_value, out_value};
  return env->funcall(env, env->intern(env, "list"), 2, values);
}
static void register_net_speed(emacs_env *env) {
  init_net_speed();

  emacs_value func = env->make_function(
      env, 0, 0, net_speed,
      "Return network speed as a list of floating point number.\n"
      "\n"
      "The first element is the download speed and the second one is upload.\n"
      "It is calculated by this formula:\n"
      "(current traffic - previous traffic) / interval time\n"
      "Start timing from the previous call of this function.",
      NULL);
  emacs_value symbol = env->intern(env, "thattem-net-speed");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

int emacs_module_init(struct emacs_runtime *runtime) {
  emacs_env *env = runtime->get_environment(runtime);

  register_cpu_usage(env);
  register_mem_usage(env);
  register_swap_usage(env);
  register_net_speed(env);

  return 0;
}
