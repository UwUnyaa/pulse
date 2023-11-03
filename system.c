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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "constants.h"
#include "fail.h"

bool directoryExists (char *dirname) {
  struct stat statResult;
  int error = stat(dirname, &statResult);
  return (error != -1 && S_ISDIR(statResult.st_mode));
}

bool fileExists (char *filename) {
  struct stat statResult;
  int error = stat(filename, &statResult);
  return error == 0;
}

bool systemIsWSL (void) {
  return fileExists("/proc/sys/fs/binfmt_misc/WSLInterop");
}

long wslGetMaxCPUFrequency (void) {
  char buffer[BUFSIZE];
  FILE *shell = popen("powershell.exe -c \"echo (Get-CimInstance CIM_Processor).MaxClockSpeed\"", "r");
  if (!shell) {
    fail("wslGetMaxFrequency(): popen() failed");
  }

  char *fgetsResult = fgets(buffer, sizeof (buffer), shell);
  if (!fgetsResult) {
    fail("wslGetMaxFrequency(): fgets() failed");
  }

  long frequency = atol(buffer) * 1000; /* number is in MHz, normalize */
  pclose(shell);
  return frequency;
}

bool userIsRoot (void) {
  return getuid() == 0;
}
