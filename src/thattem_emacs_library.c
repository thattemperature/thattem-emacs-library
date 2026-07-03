#include <emacs-module.h>
#include <gdbus_control.h>
#include <gtop_data.h>

int plugin_is_GPL_compatible;

// --- gtop ---

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

// --- gdbus ---

static emacs_value workspace_switch(emacs_env *env, ptrdiff_t nargs,
                                    emacs_value args[], void *data) {
  GError *error = NULL;
  intmax_t index = env->extract_integer(env, args[0]);

  int result = switch_workspace_to(index, &error);

  if (error)
    g_error_free(error);

  if (result)
    return env->intern(env, "nil");
  else
    return env->intern(env, "t");
}
static void register_workspace_switch(emacs_env *env) {
  emacs_value func = env->make_function(
      env, 1, 1, workspace_switch,
      "Switch to the workspace with INDEX.\n"
      "\n"
      "The INDEX should be zero-based.\n"
      "And calling this function with illegal INDEX does nothing.\n"
      "\n"
      "(fn INDEX)",
      NULL);
  emacs_value symbol = env->intern(env, "thattem-workspace-switch");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

static emacs_value workspace_active(emacs_env *env, ptrdiff_t nargs,
                                    emacs_value args[], void *data) {
  GError *error = NULL;

  int result = get_active_workspace(&error);

  if (error)
    g_error_free(error);

  if (result >= 0)
    return env->make_integer(env, result);
  else
    return env->intern(env, "nil");
}
static void register_workspace_active(emacs_env *env) {
  emacs_value func =
      env->make_function(env, 0, 0, workspace_active,
                         "Return the index of the active workspace.\n"
                         "\n"
                         "The index will be zero-based.",
                         NULL);
  emacs_value symbol = env->intern(env, "thattem-workspace-active");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

static emacs_value workspace_count(emacs_env *env, ptrdiff_t nargs,
                                   emacs_value args[], void *data) {
  GError *error = NULL;

  int result = get_workspace_count(&error);

  if (error)
    g_error_free(error);

  if (result >= 0)
    return env->make_integer(env, result);
  else
    return env->intern(env, "nil");
}
static void register_workspace_count(emacs_env *env) {
  emacs_value func = env->make_function(
      env, 0, 0, workspace_count, "Return the number of workspace.", NULL);
  emacs_value symbol = env->intern(env, "thattem-workspace-count");
  emacs_value args[] = {symbol, func};
  env->funcall(env, env->intern(env, "defalias"), 2, args);
}

int emacs_module_init(struct emacs_runtime *runtime) {
  emacs_env *env = runtime->get_environment(runtime);

  register_cpu_usage(env);
  register_mem_usage(env);
  register_swap_usage(env);
  register_net_speed(env);

  register_workspace_switch(env);
  register_workspace_active(env);
  register_workspace_count(env);

  return 0;
}
