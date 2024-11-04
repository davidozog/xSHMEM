/* Copyright (C) 2024 Intel Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "xSHMEM.h"
#include <CL/sycl.hpp>

constexpr size_t N = 10;

int main() {
    /* Example: select standard OpenSHMEM */
    //shmem_library_t shmem_lib = shmem_library_t::SHMEM;

    /* Example: select NVSHMEM */
    //shmem_library_t shmem_lib = shmem_library_t::NVSHMEM;

    /* Create an instance of a standard OpenSHMEM implementation */

    //auto shmem = shmem_create_library(shmem_lib);

    sycl::queue q;

    /* Or create with an environment variable: */
    //auto shmem = shmem_create_library();

    /* Use the selected SHMEM interface */
    //shmem->init();

    //std::cout << "My PE: " << shmem->my_pe()
    //          << " , Selected device: " << q.get_device().get_info<sycl::info::device::name>()
    //          << std::endl;

    //// Allocate memory on the ISHMEM symmetric heap on the device
    //int *src = (int *) shmem->malloc(N * sizeof(int));
    //int *dst = (int *) shmem->malloc(N * sizeof(int));

    OpenSHMEM2<IshmemAPI> *shmem;
    shmem->init();

    std::cout << "My PE: " << shmem->my_pe()
              << " , Selected device: " << q.get_device().get_info<sycl::info::device::name>()
              << std::endl;

    int *src = (int *) shmem->malloc(N * sizeof(int));

    // Initialize the src array on each device with the value pe
    q.parallel_for(sycl::range<1>(N), [=](sycl::item<1> id) { src[id] = shmem->my_pe(); }).wait();

    /* ... perform the appropriate SHMEM operations ... */
    //shmem->finalize();

    shmem->free(src);
    shmem->finalize();

    return 0;
}
