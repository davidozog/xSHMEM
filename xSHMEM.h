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
#endif

#ifdef ENABLE_ISHMEM
#include <ishmem.h>
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

#ifdef ENABLE_SHMEM
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

    /* TODO: all other OpenSHMEM routines.. */
};
#endif /* ENABLE_SHMEM */

#ifdef ENABLE_NVSHMEM
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

    // Implement other NVSHMEM-specific routines
    int _init_attr(unsigned int flags, nvshmemx_init_attr_t *attributes) {
        return nvshmemx_init_attr(flags, attributes);
    }
};
#endif /* ENABLE_NVSHMEM */

#ifdef ENABLE_ISHMEM
class ISHMEMImplementation : public OpenSHMEM {
public:
    void init() override {
        return ishmem_init();
    }

    void finalize() override {
        return ishmem_finalize();
    }

    int my_pe() override {
        return ishmem_my_pe();
    }

    int n_pes() override {
        return ishmem_n_pes();
    }

    void put(int *dest, const int *source, size_t nelems, int pe) override {
        return ishmem_int_put(dest, source, nelems, pe);
    }

    /* Implement other NVSHMEM-specific routines ? */
    void _init_attr(ishmemx_attr_t *attr) {
        return ishmem_init_attr(attr);
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

std::unique_ptr<OpenSHMEM> shmem_create_library(void) {
    char* library_name = std::getenv("SHMEM_LIBRARY");

    if (!library_name) {
        std::cout << "SHMEM_LIBRARY env var unset, picking SHMEM." << std::endl;
        char shmem_lib[6] = "";
        strcat(shmem_lib, "SHMEM");
        library_name = shmem_lib;
    }

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
