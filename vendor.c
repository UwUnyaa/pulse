/*
  Copyright 2017-2019 DoMiNeLa10

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

#include <stddef.h>
#include <string.h>

#include "macros.h"
#include "vendor-constants.h"

void normalizeVendorName (char *name, size_t len) {
  for (size_t i = 0; i < lengthof (vendorNames); i += 1) {
    if (strcmp(vendorNames[i].cpuid, name) == 0) {
      strncpy(name, vendorNames[i].name, len);
      return;
    }
  }
}
