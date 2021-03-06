/*
  Copyright 2017-2020 UwUnyaa

  This file is part of pulse.

  pulse is free software: you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.

  pulse is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  pulse. If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#include "interface-types.h"
#include "types.h"
#include "cpu-types.h"
#include "gpointer-structs.h"
#include "macros.h"

#include "cpu.h"

void lockToggle (GtkWidget *button, bool *state) {
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button),
                               (gboolean) *state);
}

void toggleHandler (GtkWidget *button, struct CPUEnableInfo *data) {
  bool
    newState = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (button)),
    result = setCPUEnableState(newState, data->info, data->nthCPU);

  printf("Setting cpu%u to %s: %s\n", data->nthCPU,
         (newState ? "enabled" : "disabled"),
         (result ? "succeeded" : "failed"));
}

gint updateUsage (struct CPUStatsAndInterface *data) {
  getCPUStats(data->info, data->count);
  for (CPUCount_t i = 0; i < data->count; i += 1) {
    double usage = clamp(getCPUUsage(&data->info[i]), 0.0, 1.0);
    gtk_progress_bar_update(GTK_PROGRESS_BAR (data->interface[i].usageBar),
                            (gdouble) usage);
  }

  return TRUE;
}
