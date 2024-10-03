/* Copyright (C) 2024 Intel Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "xSHMEM.h"

int main() {
    /* Example: select standard OpenSHMEM */
    //shmem_library_t shmem_lib = shmem_library_t::SHMEM;

    /* Example: select NVSHMEM */
    //shmem_library_t shmem_lib = shmem_library_t::NVSHMEM;

    /* Create an instance of a standard OpenSHMEM implementation */

    //auto shmem = shmem_create_library(shmem_lib);

    /* Or create with an environment variable: */
    auto shmem = shmem_create_library();

    /* Use the selected SHMEM interface */
    shmem->init();

    std::cout << "Hello from PE " << shmem->my_pe() << " out of " <<
                 shmem->n_pes() << std::endl;

    /* ... perform the appropriate SHMEM operations ... */
    shmem->finalize();

    return 0;
}
