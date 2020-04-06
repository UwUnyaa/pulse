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

#include <stdbool.h>

#include "cpu-types.h"
#include "types.h"

CPUCount_t getCPUCount (void);
int getCpuinfoField (char *field, char *result, size_t len);
long getCPUMaxFrequency (void);
bool getCPUEnableState (CPUCount_t nthCPU);
bool setCPUEnableState (bool state,
                        struct ProcessorInfo *info,
                        CPUCount_t nthCPU);
void getCPUStats (struct ProcessorInfo *info, CPUCount_t count);
double getCPUUsage (struct ProcessorInfo *info);
void initInfos (struct ProcessorInfo *stat, CPUCount_t count);
