/*
  Copyright 2017 DoMiNeLa10

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

#include <stdio.h>
#include <string.h>             /* memcpy() */
#include <pango/pangocairo.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stddef.h>

#include "constants.h"
#include "badge.h"
#include "macros.h"

/* cpu.c */
int getCpuinfoField (char *field, char *result, size_t len);
long getCPUMaxFrequency (void);

/* vendor.c */
void normalizeVendorName (char *name, size_t len);

static cairo_status_t readPNGData (void *closure, unsigned char *buffer,
                                   unsigned int length) {
  ignore(closure);
  static unsigned char *pos = cpuTemplate;

  if (pos + length > cpuTemplate + lengthof (cpuTemplate)) {
    return CAIRO_STATUS_READ_ERROR;
  }

  memcpy(buffer, pos, length);
  pos += length;

  return CAIRO_STATUS_SUCCESS;
}

static void normalizeCPUFrequency (long frequency, char *buf) {
  unsigned int powers = 0;
  double result = frequency;

  while (result > 1000) {
    result /= 1000;
    powers += 1;
  }

  if (powers > lengthof (cpuFrequencyPowers)) {
    result = frequency;
    powers = 0;
  }

  sprintf(buf, "%#.2f %sHz", result, cpuFrequencyPowers[powers]);
}

static void drawBadgeText (cairo_t *cr, char *label,
                           int ypos, int fontSize) {
  char fontSpec[BUFSIZE];
  sprintf(fontSpec, "sans %d", fontSize);

  PangoLayout *layout = pango_cairo_create_layout(cr);
  pango_layout_set_text(layout, label, -1);

  PangoFontDescription *description =
    pango_font_description_from_string(fontSpec);
  pango_layout_set_font_description(layout, description);
  pango_font_description_free(description);

  pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
  pango_layout_set_width(
    layout, pango_units_from_double(BadgeSize - (2 * BadgeMargin)));
  pango_layout_set_wrap(layout, PANGO_WRAP_WORD);

  pango_cairo_update_layout(cr, layout);
  cairo_move_to(cr, BadgeMargin, ypos);
  pango_cairo_show_layout(cr, layout);

  g_object_unref(layout);
}

GtkWidget* createBadgeImage (GdkDrawable *drawable) {
  GdkPixmap *pixmap = gdk_pixmap_new(drawable, BadgeSize, BadgeSize, -1);
  cairo_t *cr = gdk_cairo_create(pixmap);

  cairo_surface_t
    *pngSurface = cairo_image_surface_create_from_png_stream(readPNGData, 0);
  cairo_set_source_surface(cr, pngSurface, 0, 0);
  cairo_rectangle(cr, 0, 0, BadgeSize, BadgeSize);
  cairo_paint(cr);
  cairo_surface_destroy(pngSurface);

  cairo_set_source_rgb(cr, BadgeTextRed, BadgeTextGreen, BadgeTextBlue);

  char line[BUFSIZE];

  getCpuinfoField("vendor_id", line, sizeof (line));
  normalizeVendorName(line, sizeof (line));
  drawBadgeText(cr, line, 8, 20);

  getCpuinfoField("model name", line, sizeof (line));
  drawBadgeText(cr, line, 40, 8);

  normalizeCPUFrequency(getCPUMaxFrequency(), line);
  drawBadgeText(cr, line, 108, 8);

  cairo_destroy(cr);

  /* No idea what the second argument is even supposed to do. */
  GtkWidget *image = gtk_image_new_from_pixmap(pixmap, 0);
  return image;
}
