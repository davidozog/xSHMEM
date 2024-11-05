/* Copyright (C) 2024 Intel Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "xSHMEM.h"
#include <CL/sycl.hpp>

constexpr size_t N = 10;

int main() {

    OpenSHMEM2<IshmemAPI> *shmem;
    sycl::queue q;

    shmem->init();

    int my_pe = shmem->my_pe();
    int n_pes = shmem->n_pes();

    std::cout << "My PE: " << my_pe << " out of " << n_pes
              << " , Selected device: " << q.get_device().get_info<sycl::info::device::name>()
              << std::endl;

    int *dst = (int *) shmem->calloc(N, sizeof(int));

    // Ensure completion of initialization
    shmem->barrier_all();

    // Get data from the next device
    //q.parallel_for(sycl::range<1>(N), [=](sycl::item<1> id) {
    //     int next_pe = (my_pe + 1) % shmem->n_pes();
    //     int value = shmem->my_pe();
    //     shmem->int_p(&dst[id], value, next_pe);
    //}).wait_and_throw();

    auto e_verify = q.submit([&](sycl::handler &h) {
        h.single_task([=]() {
           int next_pe = (my_pe + 1) % shmem->n_pes();
           int value = shmem->my_pe();
           shmem->int_p(dst, value, next_pe);
        });
    });
    e_verify.wait_and_throw();

    // Ensure completion of get operations and wait for other PEs
    shmem->barrier_all();

    // Verify the data
    int *check = (int *) sycl::malloc_host<int>(N, q);
    //q.parallel_for(sycl::range<1>(N), [=](sycl::item<1> id) {
    //     int expected = my_pe == 0 ? n_pes - 1 : my_pe - 1;
    //     if (dst[id] != expected) {
    //         check[id] = 1;
    //     } else {
    //         check[id] = 0;
    //     }
    //}).wait();

    e_verify = q.submit([&](sycl::handler &h) {
        h.single_task([=]() {
            int expected = my_pe == 0 ? n_pes - 1 : my_pe - 1;
            if (dst[0] != expected) {
                check[0] = 1;
            } else {
                check[0] = 0;
            }
        });
    });
    e_verify.wait_and_throw();

    bool check_fail = false;
    for (size_t i = 0; i < 1; i++)
    //for (size_t i = 0; i < N; i++)
        if (check[i]) {
            std::cerr << "PE: " << my_pe << " SHMEM get failed at index: " << i << std::endl;
            check_fail = true;
            continue;
        }

    if (!check_fail)
        std::cout << my_pe << " PE successfully received the data using SHMEM get." << std::endl;

    fflush(stdout);

    free(check, q);
    shmem->free(dst);
    shmem->finalize();

    return 0;
}
