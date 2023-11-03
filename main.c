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
#include <stdlib.h>

#include "types.h"
#include "interface-types.h"
#include "cpu-types.h"
#include "gpointer-structs.h"
#include "config.h"

#include "cpu.h"
#include "interface.h"
#include "events.h"
#include "fail.h"
#include "system.h"

int main (int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  CPUCount_t CPUCount = getCPUCount();
  bool isRoot = userIsRoot();
  bool isWSL = systemIsWSL();

  struct CPUInterface *interfaces =
    malloc(sizeof (struct CPUInterface) * CPUCount);
  if (!interfaces) {
    fail("main(): couldn't malloc() interface struct array");
  }

  struct ProcessorInfo *info =
    malloc(sizeof (struct ProcessorInfo) * CPUCount);
  if (!info) {
    fail("main(): couldn't malloc() cpu struct array");
  }

  initInfos(info, CPUCount);
  getCPUStats(info, CPUCount);

  GtkWidget *window = initInterface(interfaces, CPUCount);
  if (!window) {
    fail("main(): initInterface() returned a null pointer");
  }

  struct CPUStatsAndInterface CPUData;
  CPUData.info = info;
  CPUData.interface = interfaces;
  CPUData.count = CPUCount;

  struct CPUEnableInfo *callbackData =
    malloc(sizeof (struct CPUEnableInfo) * CPUCount);
  if (isRoot) {
    for (CPUCount_t i = 0; i < CPUCount; i += 1) {
      callbackData[i].info = info;
      callbackData[i].nthCPU = i;
    }
  }

  /* bind callbacks to toggles */
  g_signal_connect(interfaces[0].toggle, "toggled",
                   G_CALLBACK (lockToggle), &info[0].enabled);
  if (isRoot && !isWSL) {    /* writing to online files in WSL does nothing */
    for (CPUCount_t i = 1; i < CPUCount; i += 1) {
      g_signal_connect(interfaces[i].toggle, "toggled",
                       G_CALLBACK (toggleHandler), &callbackData[i]);
    }
  } else {
    for (CPUCount_t i = 1; i < CPUCount; i += 1) {
      g_signal_connect(interfaces[i].toggle, "toggled",
                       G_CALLBACK (lockToggle), &info[i].enabled);
    }
  }

  /* update toggle button states */
  for (CPUCount_t i = 1; i < CPUCount; i += 1) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (interfaces[i].toggle),
                                 (gboolean) info[i].enabled);
  }
  /* add event handlers */
  g_signal_connect(window, "delete-event", G_CALLBACK (gtk_main_quit), 0);
  g_timeout_add(refreshInterval, (GSourceFunc) updateUsage, &CPUData);

  gtk_main();
  return 0;
}
