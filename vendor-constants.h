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

struct vendorName {
  char *cpuid;
  char *name;
};

/* https://en.wikipedia.org/wiki/CPUID#EAX.3D0:_Get_vendor_ID_.28including_EAX.3D1:_Get_CPUID.29 */
static const struct vendorName vendorNames[] = {
  {"AMDisbetter!", "AMD"},
  {"AuthenticAMD", "AMD"},
  {"CentaurHauls", "Centaur"},
  {"CyrixInstead", "Cyrix"},
  {"GenuineIntel", "Intel"},
  {"TransmetaCPU", "Transmeta"},
  {"GenuineTMx86", "Transmeta"},
  {"Geode by NSC", "National Semiconductor"},
  {"NexGenDriven", "NexGen"},
  {"RiseRiseRise", "Rise"},
  {"SiS SiS SiS ", "SiS"},
  {"UMC UMC UMC ", "UMC"},
  {"VIA VIA VIA ", "VIA"},
  {"Vortex86 SoC", "Vortex"},
  {"bhyve bhyve ", "bhyve"},
  {"KVMKVMKVM",    "KVM"},
  {"Microsoft Hv", "Hyper-V"},
  {" lrpepyh vr",  "Parallels"},
  {"VMwareVMware", "VMware"},
  {"XenVMMXenVMM", "Xen HVM"},
};
