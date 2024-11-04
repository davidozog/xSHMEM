/* Copyright (C) 2024 Intel Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "config.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <cstring>

#ifdef ENABLE_SHMEM
#include <shmem.h>
#endif

#ifdef ENABLE_NVSHMEM
#include <nvshmem.h>
#include <nvshmemx.h>
#endif

#ifdef ENABLE_ISHMEM
#include <ishmem.h>
#include <ishmemx.h>
#endif

#ifdef ENABLE_ROCSHMEM
#include <roc_shmem.hpp>
#endif


class OpenSHMEM {
public:
    virtual ~OpenSHMEM() {}

    /* Define virtual functions for OpenSHMEM operations */
    virtual void init() = 0;
    virtual void finalize() = 0;
    virtual int my_pe() = 0;
    virtual int n_pes() = 0;
    virtual void put(int *dest, const int *source, size_t nelems, int pe) = 0;
    virtual void* malloc(size_t size) = 0;

    /* TODO: all other OpenSHMEM routines.. */

    /* TODO: Add other (e.g. vendor-specific) SHMEM extension routines as needed ? */
#ifdef ENABLE_SHMEMX
#ifdef ENABLE_NVSHMEM
    virtual int _init_attr(unsigned int flags, nvshmemx_init_attr_t *attributes) = 0;
#elif ENABLE_ISHMEM
    virtual void _init_attr(ishmemx_attr_t *attr) = 0;
#endif
#endif /* ENABLE_SHMEMX */
};

template <typename API>
class OpenSHMEM2 {
public:

    OpenSHMEM2() = default;
    ~OpenSHMEM2() = default;

    void init(void) {
        API::init();
    }
    void finalize(void) {
        API::finalize();
    }
    int my_pe(void) {
        return API::my_pe();
    }
    //int my_pe() = 0;
    //int n_pes() = 0;
    //void put(int *dest, const int *source, size_t nelems, int pe) = 0;
    //void* malloc(size_t size) = 0;

    void *malloc(size_t size) {
        return API::malloc(size);
    }

    void free(void *ptr) {
        return API::free(ptr);
    }

    /* TODO: all other OpenSHMEM routines.. */
    /* TODO: Add other (e.g. vendor-specific) SHMEM extension routines as needed ? */

};

#ifdef ENABLE_SHMEM
// Specific API implementations
class ShmemAPI {
public:
    static void init() {
        std::cout << "Initializing shmem_init API" << std::endl;
        shmem_init();
    }

    static void finalize() {
        std::cout << "Finalizing shmem_finalize API" << std::endl;
        shmem_finalize();
    }

    static void *malloc(size_t size) {
        std::cout << "Calling shmem_malloc API" << std::endl;
        return shmem_malloc(size);
    }

    static void free(void *ptr) {
        std::cout << "Calling shmem_free API" << std::endl;
        return shmem_free(ptr);
    }

    static int my_pe(void) {
        std::cout << "Calling shmem_my_pe API" << std::endl;
        return shmem_my_pe();
    }

    // Add other shmem_* specific methods here
    // ...
};

class OpenSHMEMImplementation : public OpenSHMEM {
public:
    void init() override {
        return shmem_init();
    }

    void finalize() override {
        return shmem_finalize();
    }

    int my_pe() override {
        return shmem_my_pe();
    }

    int n_pes() override {
        return shmem_n_pes();
    }

    void put(int *dest, const int *source, size_t nelems, int pe) override {
        return shmem_int_put(dest, source, nelems, pe);
    }

    void* malloc(size_t size) override {
        return shmem_malloc(size);
    }

    /* TODO: all other OpenSHMEM routines.. */
};
#endif /* ENABLE_SHMEM */

#ifdef ENABLE_NVSHMEM

class NvshmemAPI {
public:
    static void init() {
        std::cout << "Initializing nvshmem_init API" << std::endl;
        nvshmem_init();
    }

    static void finalize() {
        std::cout << "Finalizing nvshmem_finalize API" << std::endl;
        nvshmem_finalize();
    }

    static void *malloc(size_t size) {
        std::cout << "Calling nvshmem_malloc API" << std::endl;
        return nvshmem_malloc(size);
    }

    static void free(void *ptr) {
        std::cout << "Calling nvshmem_free API" << std::endl;
        return nvshmem_free(ptr);
    }

    static int my_pe(void) {
        std::cout << "Calling nvshmem_my_pe API" << std::endl;
        return nvshmem_my_pe();
    }
};

class NVSHMEMImplementation : public OpenSHMEM {
public:
    void init() override {
        return nvshmem_init();
    }

    void finalize() override {
        return nvshmem_finalize();
    }

    int my_pe() override {
        return nvshmem_my_pe();
    }

    int n_pes() override {
        return nvshmem_n_pes();
    }

    void put(int *dest, const int *source, size_t nelems, int pe) override {
        return nvshmem_int_put(dest, source, nelems, pe);
    }

    void* malloc(size_t size) override {
        return nvshmem_malloc(size);
    }

    // Implement other NVSHMEM-specific routines
    int _init_attr(unsigned int flags, nvshmemx_init_attr_t *attributes) {
        return nvshmemx_init_attr(flags, attributes);
    }
};
#endif /* ENABLE_NVSHMEM */

#ifdef ENABLE_ISHMEM
class IshmemAPI {
public:
    static void init() {
        std::cout << "Initializing ishmem_init API" << std::endl;
        ishmem_init();
    }

    static void finalize() {
        std::cout << "Finalizing ishmem_finalize API" << std::endl;
        ishmem_finalize();
    }

    static void *malloc(size_t size) {
        std::cout << "Calling ishmem_malloc API" << std::endl;
        return ishmem_malloc(size);
    }

    static void free(void *ptr) {
        std::cout << "Calling ishmem_free API" << std::endl;
        return ishmem_free(ptr);
    }

#if __SYCL_DEVICE_ONLY__
    static SYCL_EXTERNAL int my_pe(void) {
        ishmemx_print("Calling ishmem_my_pe API (device)\n");
        return ishmem_my_pe();
    }
#else
    static int my_pe(void) {
        std::cout << "Calling ishmem_my_pe API (host)" << std::endl;
        return ishmem_my_pe();
    }
#endif
};

class ISHMEMImplementation : public OpenSHMEM {
public:
    void init() override {
        return ishmem_init();
    }

    void finalize() override {
        return ishmem_finalize();
    }

#if __SYCL_DEVICE_ONLY__
    SYCL_EXTERNAL int my_pe() override {
        return ishmem_my_pe();
    }
#else
    int my_pe() override {
        return ishmem_my_pe();
    }
#endif

    int n_pes() override {
        return ishmem_n_pes();
    }

    void put(int *dest, const int *source, size_t nelems, int pe) override {
        return ishmem_int_put(dest, source, nelems, pe);
    }

    void* malloc(size_t size) override {
        return ishmem_malloc(size);
    }

    /* Implement other NVSHMEM-specific routines ? */
    void _init_attr(ishmemx_attr_t *attr) {
        return ishmemx_init_attr(attr);
    }
};
#endif /* ENABLE_ISHMEM */

/* Derived classes for other SHMEM implementations ? */

enum class shmem_library_t {
    SHMEM,
    NVSHMEM,
    ISHMEM,
    ROCSHMEM,
    /* other SHMEM types? */
};

std::unique_ptr<OpenSHMEM> shmem_create_library(shmem_library_t type) {
    switch (type) {
#ifdef ENABLE_SHMEM
        case shmem_library_t::SHMEM:
            return std::make_unique<OpenSHMEMImplementation>();
#endif
#ifdef ENABLE_NVSHMEM
        case shmem_library_t::NVSHMEM:
            return std::make_unique<NVSHMEMImplementation>();
#endif
#ifdef ENABLE_ISHMEM
        case shmem_library_t::ISHMEM:
            return std::make_unique<ISHMEMImplementation>();
#endif
#ifdef ENABLE_ROCSHMEM
        case shmem_library_t::ROCSHMEM:
            return std::make_unique<ROCSHMEMImplementation>();
#endif
        default:
            throw std::invalid_argument("Unsupported SHMEM type");
    }
}

std::unique_ptr<OpenSHMEM> shmem_create_library(const char *library_name) {
#ifdef ENABLE_SHMEM
    if (strcmp(library_name, "SHMEM") == 0)
        return std::make_unique<OpenSHMEMImplementation>();
#endif
#ifdef ENABLE_NVSHMEM
    if (strcmp(library_name, "NVSHMEM") == 0)
        return std::make_unique<NVSHMEMImplementation>();
#endif
#ifdef ENABLE_ISHMEM
    if (strcmp(library_name, "ISHMEM") == 0)
        return std::make_unique<ISHMEMImplementation>();
#endif
#ifdef ENABLE_ROCSHMEM
    if (strcmp(library_name, "ROCSHMEM") == 0)
        return std::make_unique<ROCSHMEMImplementation>();
#endif
    else
        throw std::invalid_argument("Unsupported SHMEM type");
}

OpenSHMEM* shmem_create_library(void) {
    char* library_name = std::getenv("SHMEM_LIBRARY");

    if (!library_name) {
        std::cout << "SHMEM_LIBRARY env var unset, picking SHMEM." << std::endl;
        char shmem_lib[6] = "";
        strcat(shmem_lib, "SHMEM");
        library_name = shmem_lib;
    }

#ifdef ENABLE_SHMEM
    if (strcmp(library_name, "SHMEM") == 0)
        return OpenSHMEMImplementation();
#endif
#ifdef ENABLE_NVSHMEM
    if (strcmp(library_name, "NVSHMEM") == 0)
        return NVSHMEMImplementation();
#endif
#ifdef ENABLE_ISHMEM
    if (strcmp(library_name, "ISHMEM") == 0)
        return new ISHMEMImplementation();
#endif
#ifdef ENABLE_ROCSHMEM
    if (strcmp(library_name, "ROCSHMEM") == 0)
        return ROCSHMEMImplementation();
#endif
    else
        throw std::invalid_argument("Unsupported SHMEM type");
}
