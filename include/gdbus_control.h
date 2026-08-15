#ifndef GDBUS_CONTROL_H
#define GDBUS_CONTROL_H

#include <glib.h>

int switch_workspace_to(int index, GError **error);

int get_active_workspace(GError **error);

int get_workspace_count(GError **error);

#endif
