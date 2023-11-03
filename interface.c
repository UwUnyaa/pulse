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

#include "interface-types.h"
#include "types.h"
#include "constants.h"
#include "config.h"

#include "badge.h"

GtkWidget* initInterface (struct CPUInterface *interfaces, CPUCount_t CPUCount) {
  /* INTERFACES should be malloc()ed */
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_resizable(GTK_WINDOW (window), FALSE);
  gtk_window_set_title(GTK_WINDOW (window), windowLabel);
  gtk_container_set_border_width(GTK_CONTAINER (window), borderSize);

  GtkWidget *topHbox = gtk_hbox_new(FALSE, borderSize);
  gtk_container_add(GTK_CONTAINER (window), topHbox);
  gtk_widget_show(topHbox);


  GtkWidget *imageAlignment = gtk_alignment_new(0, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX (topHbox), imageAlignment,
                     FALSE, FALSE, borderSize);
  gtk_widget_show(imageAlignment);

  GtkWidget *CPUsVbox = gtk_vbox_new(TRUE, (2 * borderSize));
  gtk_box_pack_start(GTK_BOX (topHbox), CPUsVbox, TRUE, TRUE, borderSize);
  gtk_widget_show(CPUsVbox);

  for (CPUCount_t i = 0; i < CPUCount; i += 1) {
    char CPULabel[BUFSIZE];
    sprintf(CPULabel, "%u", i);

    struct CPUInterface *current = &interfaces[i];
    GtkWidget *hbox = gtk_hbox_new(FALSE, borderSize);

    gtk_box_pack_start(GTK_BOX (CPUsVbox), hbox, FALSE, TRUE, 0);

    current->toggle = gtk_toggle_button_new_with_label(CPULabel);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (current->toggle), TRUE);
    current->usageBar = gtk_progress_bar_new();
    gtk_progress_bar_update(GTK_PROGRESS_BAR (current->usageBar), 0);

    gtk_box_pack_start(GTK_BOX (hbox), current->toggle, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX (hbox), current->usageBar, TRUE, TRUE, 0);

    gtk_widget_show(hbox);
    gtk_widget_show(current->toggle);
    gtk_widget_show(current->usageBar);
  }

  gtk_widget_show(CPUsVbox);

  /* draw the badge as the last thing, to avoid showing a blank window */
  /* show window first, in order to make it drawable */
  gtk_widget_show(window);
  /* no clue what I'm doing, but this compiles */
  GtkWidget *image = createBadgeImage(window->window);
  gtk_container_add(GTK_CONTAINER (imageAlignment), image);
  gtk_widget_show(image);

  return window;
}
