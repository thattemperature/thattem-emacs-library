#include <emacs-module.h>
#include <gtop_data.h>

int plugin_is_GPL_compatible;

static emacs_value cpu_usage(emacs_env *env, ptrdiff_t nargs,
                             emacs_value args[], void *data) {
  return env->make_float(env, get_cpu_usage());
}
static void register_cpu_usage(emacs_env *env) {
  init_cpu_usage();

  emacs_value func =
      env->make_function(env, 0, 0, cpu_usage,
                         "Return CPU usage as a floating point number.", NULL);
  emacs_value symbol = env->intern(env, "thattem-cpu-usage");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

static emacs_value mem_usage(emacs_env *env, ptrdiff_t nargs,
                             emacs_value args[], void *data) {
  return env->make_float(env, get_mem_usage());
}
static void register_mem_usage(emacs_env *env) {
  emacs_value func = env->make_function(
      env, 0, 0, mem_usage, "Return memory usage as a floating point number.",
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
                         "Return swap usage as a floating point number.", NULL);
  emacs_value symbol = env->intern(env, "thattem-swap-usage");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

int emacs_module_init(struct emacs_runtime *runtime) {
  emacs_env *env = runtime->get_environment(runtime);

  register_cpu_usage(env);
  register_mem_usage(env);
  register_swap_usage(env);

  return 0;
}
