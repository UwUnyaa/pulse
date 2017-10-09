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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "constants.h"
#include "cpu.h"

/* fail.c */
void fail(char*);

/* system.c */
bool directoryExists (char*);
bool userIsRoot (void);

CPUCount_t getCPUCount (void) {
  CPUCount_t count = 1;
  char dirName[BUFSIZE];

  for (;;) {
    if (count >= MAXCORES) {
      fail("too many processors, something is wrong");
    }

    sprintf(dirName, CPUSysInfoDirFormat, count);
    if (directoryExists(dirName)) {
      count += 1;
    } else {
      break;
    }
  }

  return count;
}

int getCpuinfoField (char *field, char *result, size_t len) {
  FILE *file = fopen("/proc/cpuinfo", "r");
  if (!file) {
    return 0;
  }

  char line[BUFSIZE], fieldName[BUFSIZE], fieldVal[BUFSIZE];
  int isEof;

  do {
    char *fgetsResult = fgets(line, sizeof (line), file);

    if (!fgetsResult) {
      fail("getCpuinfoField(): fgets() failed");
    }

    sscanf(line, "%4096[^\t]%*[\t: ]%4096[^\n]", fieldName, fieldVal);
  } while (strcmp(field, fieldName) != 0 || (isEof = feof(file)));

  if (isEof) {
    return 0;
  }

  strncpy(result, fieldVal, len);
  return 1;
}

bool getCPUEnableState (CPUCount_t nthCPU) {
  if (nthCPU == 0) {     /* cpu 0 can't be disabled */
    return true;
  }

  if (nthCPU >= MAXCORES) {
    fail("getCPUEnableState(): argument out of range");
  }

  char fileName[BUFSIZE];
  sprintf(fileName, CPUEnabledFileFormat, nthCPU);

  FILE *file = fopen(fileName, "r");
  if (!file) {
    fail("getCPUEnableState(): couldn't open file");
  }

  bool result = fgetc(file) == '1';

  fclose(file);
  return result;
}

bool setCPUEnableState (bool state,
                        struct ProcessorInfo *info,
                        CPUCount_t nthCPU) {
  /* Returns true if write succeeded. Updates the info struct. */
  if (nthCPU == 0) {
    return false;
  }

  char fileName[BUFSIZE];
  sprintf(fileName, CPUEnabledFileFormat, nthCPU);

  FILE *file = fopen(fileName, "w");
  if (!file) {
    return false;
  }

  int result = fputs((state ? "1" : "0"), file);
  if (result == EOF) {
    return false;
  }

  fclose(file);
  info[nthCPU].enabled = state;
  return true;
}

void getCPUStats (struct ProcessorInfo *info, CPUCount_t count) {
  FILE *statFile = fopen(CPUStatFile, "r");
  if (!statFile) {
    fail("getCPUStats(): couldn't open " CPUStatFile);
  }

  char buf[BUFSIZE], firstField[BUFSIZE];
  /* skip the first line — it's irrelevant */
  char *fgetsResult = fgets(buf, sizeof (buf), statFile);
  if (!fgetsResult) {
    fail("getCPUStats(): fgets() failed");
  }

  for (CPUCount_t iterations = 0; iterations < count; iterations += 1) {
    struct ProcessorStat temp;
    CPUCount_t currentCPU;

    char *result = fgets(buf, sizeof (buf), statFile);
    if (!result) {
      fail("getCPUStats(): fgets() failed");
    }

    sscanf(buf, "%3[^0-9]%u %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
           firstField,
           &currentCPU,
           &temp.user,
           &temp.nice,
           &temp.system,
           &temp.idle,
           &temp.iowait,
           &temp.irq,
           &temp.softirq,
           &temp.steal,
           &temp.guest,
           &temp.guest_nice);

    if (strcmp(firstField, "cpu") != 0 || currentCPU >= count) {
      /* all cpu fields were read already */
      break;
    }

    memcpy(&info[currentCPU].previousStat,
           &info[currentCPU].currentStat,
           sizeof (struct ProcessorStat));

    memcpy(&info[currentCPU].currentStat,
           &temp,
           sizeof (struct ProcessorStat));
  }
  fclose(statFile);
}

double getCPUUsage (struct ProcessorInfo *info) {
  double usage = 0;
  if (info->enabled) {
    struct ProcessorStat
      *current = &info->currentStat,
      *previous = &info->previousStat;

    double work =
      (current->user     +
       current->nice     +
       current->system   +
       current->irq      +
       current->softirq  +
       current->guest    +
       current->guest_nice) -
      (previous->user    +
       previous->nice    +
       previous->system  +
       previous->irq     +
       previous->softirq +
       previous->guest   +
       previous->guest_nice);

    double total = work +
      ((current->idle   - previous->idle)  +
       (current->steal  - previous->steal) +
       (current->iowait - current->iowait));

    usage = work / total;
  }

  info->usage = usage;
  return usage;
}


static void initStat (struct ProcessorStat *stat) {
  stat->user       = 0;
  stat->nice       = 0;
  stat->system     = 0;
  stat->idle       = 0;
  stat->iowait     = 0;
  stat->irq        = 0;
  stat->softirq    = 0;
  stat->steal      = 0;
  stat->guest      = 0;
  stat->guest_nice = 0;
}

static void initInfo (struct ProcessorInfo *processor, CPUCount_t nthCPU) {
  processor->enabled = getCPUEnableState(nthCPU);
  processor->usage = 0;
  initStat(&processor->previousStat);
  initStat(&processor->currentStat);
}

void initInfos (struct ProcessorInfo *stat, CPUCount_t count) {
  for (CPUCount_t i = 0; i < count; i += 1) {
    initInfo(&stat[i], i);
  }
}
