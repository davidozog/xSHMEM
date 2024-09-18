===============================================================================
SUMMARY:
===============================================================================

This is an experiment to evaluate the possibility and utility of a future
OpenSHMEM API (version 2.0?) that consists of a standardized derived base class
that vendor implementations must inherit.

This project enables a subset (proof-of-concept) of either the standard
OpenSHMEM API or one of the following vendor-specific SHMEM libraries:
  * Intel SHMEM
  * NVSHMEM
  * ROC_SHMEM

===============================================================================
BUILD INSTRUCTIONS:
===============================================================================

-------------------------------------
Sandia OpenSHMEM (SOS) on Perlmutter:
-------------------------------------
First, build and install SOS to support the CXI provider following these
instructions:
https://github.com/Sandia-OpenSHMEM/SOS/wiki/Slingshot-(CXI)-Build-Instructions

Assuming the SHMEM_INSTALL_PREFIX environment variable is set to the SOS
installation directory, the following build enables an SOS backend:

$ mkdir build
$ cd build
$ cmake .. -DSHMEM_ROOT=${SHMEM_INSTALL_PREFIX}
$ make

---------------------------------------------------------------------------
Intel SHMEM (ISHMEM) on Sunspot at the Argonne Leadership Compute Facility:
---------------------------------------------------------------------------
Assuming the ISHMEM_INSTALL_PREFIX environment variable is set to the ISHMEM
installation directory, the following build enables an ISHMEM backend:

$ mkdir build-ishmem
$ cd build-ishmem
$ cmake .. -DENABLE_SHMEM=OFF -DENABLE_ISHMEM=ON \
           -DISHMEM_ROOT=${ISHMEM_INSTALL_PREFIX}

----------------------
NVSHMEM on Perlmutter:
----------------------
Assuming the NVSHMEM_INSTALL_PREFIX environment variable is set to the NVSHMEM
installation directory, the following builds enables an NVSHMEM backend:

$ mkdir build-nvshmem
$ cd build-nvshmem
$ cmake .. -DENABLE_SHMEM=OFF -DENABLE_NVSHMEM=ON \
           -DNVSHMEM_ROOT=${NVSHMEM_INSTALL_PREFIX}
