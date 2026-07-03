#include <gio/gio.h>

static GDBusConnection *connection;
static GDBusConnection *ensure_connection(GError **error) {
  if (connection)
    return connection;

  connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, error);
  if (!connection)
    return NULL;
  return connection;
}

static const gchar *const bus_name = "org.gnome.Shell";
static const gchar *const object_path =
    "/org/gnome/Shell/Extensions/ThattemGnomeExtension";
static const gchar *const interface_name =
    "org.gnome.Shell.Extensions.ThattemGnomeExtension";
static GVariant *call_thattem_extension(const gchar *method_name,
                                        GVariant *parameters,
                                        const GVariantType *reply_type,
                                        GError **error) {
  connection = ensure_connection(error);
  if (!connection) {
    if (parameters)
      g_variant_unref(g_variant_ref_sink(parameters));
    return NULL;
  }

  GVariant *result = g_dbus_connection_call_sync(
      connection, bus_name, object_path, interface_name, method_name,
      parameters, reply_type, G_DBUS_CALL_FLAGS_NONE, -1, NULL, error);

  return result;
}

int switch_workspace_to(int index, GError **error) {
  const GVariantType *const reply_type = NULL;
  static const gchar *const method_name = "SwitchWorkspaceTo";
  GVariant *parameters = g_variant_new("(i)", index);

  GVariant *result =
      call_thattem_extension(method_name, parameters, reply_type, error);

  if (!result)
    return -1;

  g_variant_unref(result);
  return 0;
}

int get_active_workspace(GError **error) {
  const GVariantType *const reply_type = G_VARIANT_TYPE("(i)");
  static const gchar *const method_name = "GetActiveWorkspace";
  GVariant *parameters = NULL;

  GVariant *result =
      call_thattem_extension(method_name, parameters, reply_type, error);

  if (!result)
    return -1;

  int index;
  g_variant_get(result, "(i)", &index);
  g_variant_unref(result);
  return index;
}

int get_workspace_count(GError **error) {
  const GVariantType *const reply_type = G_VARIANT_TYPE("(i)");
  static const gchar *const method_name = "GetWorkspaceCount";
  GVariant *parameters = NULL;

  GVariant *result =
      call_thattem_extension(method_name, parameters, reply_type, error);

  if (!result)
    return -1;

  int index;
  g_variant_get(result, "(i)", &index);
  g_variant_unref(result);
  return index;
}
