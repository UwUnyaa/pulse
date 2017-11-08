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

#ifndef _cpu_types
#define _cpu_types

#define CPUStatFile          "/proc/stat"
#define CPUSysInfoDirFormat  "/sys/devices/system/cpu/cpu%u"
#define CPUEnabledFileFormat "/sys/devices/system/cpu/cpu%u/online"
#define CPUinfoFile          "/proc/cpuinfo"

#define MAXCORES             256

struct ProcessorStat {
  unsigned long user;
  unsigned long nice;
  unsigned long system;
  unsigned long idle;
  unsigned long iowait;
  unsigned long irq;
  unsigned long softirq;
  unsigned long steal;
  unsigned long guest;
  unsigned long guest_nice;
};

struct ProcessorInfo {
  struct ProcessorStat previousStat;
  struct ProcessorStat currentStat;
  bool enabled;
  double usage;
};

#endif
