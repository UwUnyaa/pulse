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

int main (int argc, char *argv[]) {
  if (argc != 3) {
    fputs("Wrong number of arguments\n", stderr);
    exit(1);
  }

  FILE
    *input  = fopen(argv[1], "r"),
    *output = fopen(argv[2], "w");

  if (!input || !output) {
    fputs("Couldn't open files\n", stderr);
    exit(1);
  }

  for (;;) {
    unsigned int byte = (unsigned int) fgetc(input);

    if (feof(input)) {
      break;
    }

    fprintf(output, "%u,", byte);
  }

  fclose(input);
  fclose(output);
  exit(0);
}
