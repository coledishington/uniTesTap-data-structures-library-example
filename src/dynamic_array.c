#include <data_structures.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INITAL_SIZE (1 << 5)
#define GROWTH_FACTOR 1.5f

static inline size_t get_idx(struct dynamic_array *da, size_t idx) {
    return idx * da->esize;
}

static inline void *get_ptr(struct dynamic_array *da, size_t idx) {
    return da->array + get_idx(da, idx);
}

static inline void get_value(struct dynamic_array *da, size_t idx,
                             void *element) {
    memcpy(element, get_ptr(da, idx), da->esize);
}

static inline void set_value(struct dynamic_array *da, size_t idx,
                             void *element) {
    memcpy(get_ptr(da, idx), element, da->esize);
}

static inline void clear_values(struct dynamic_array *da, size_t idx,
                                size_t n) {
    memset(get_ptr(da, idx), 0, n * da->esize);
}

static int ds_da_grow(struct dynamic_array *da) {
    size_t physical_size = SIZE_MAX;
    double d_physical_size;
    char *new_array;

    d_physical_size = ceil(da->psize * GROWTH_FACTOR);
    physical_size = d_physical_size;
    if (fabs(d_physical_size - physical_size) > 1.0f) {
        physical_size = SIZE_MAX;
    }

    new_array = realloc(da->array, physical_size * da->esize);
    if (!new_array) {
        return errno;
    }

    da->array = new_array;
    clear_values(da, da->psize, physical_size - da->psize);
    da->psize = physical_size;
    return 0;
}

int ds_da_create(size_t esize, struct dynamic_array **d_da) {
    struct dynamic_array *da;

    da = malloc(sizeof(*da));
    if (!da) {
        return errno;
    }

    da->array = malloc(esize * INITAL_SIZE);
    if (!da->array) {
        int err;

        err = errno;
        free(da);
        return err;
    }

    da->psize = INITAL_SIZE;
    da->esize = esize;
    da->lsize = 0;
    *d_da = da;
    return 0;
}

size_t ds_da_len(const struct dynamic_array *da) { return da->lsize; }

int ds_da_get_value(struct dynamic_array *da, size_t idx, void *element) {
    if (idx >= da->lsize) {
        return EINVAL;
    }

    get_value(da, idx, element);
    return 0;
}

int ds_da_append(struct dynamic_array *da, void *element) {
    if (da->lsize >= da->psize) {
        int err;

        err = ds_da_grow(da);
        if (err != 0) {
            return err;
        }
    }

    set_value(da, da->lsize, element);
    da->lsize++;
    return 0;
}

int ds_da_swap(struct dynamic_array *da, size_t idx1, size_t idx2) {
    if (idx1 >= da->lsize || idx2 >= da->lsize) {
        return EINVAL;
    }

    if (da->lsize == da->psize) {
        int err;

        err = ds_da_grow(da);
        if (err != 0) {
            return err;
        }
    }

    /* Use a free slot at the end for the temporary position */
    set_value(da, da->psize - 1, get_ptr(da, idx1));
    set_value(da, idx1, get_ptr(da, idx2));
    set_value(da, idx2, get_ptr(da, da->psize - 1));
    clear_values(da, da->psize - 1, 1);
    return 0;
}

int ds_da_pop(struct dynamic_array *da, void *element) {
    size_t len, idx;

    len = ds_da_len(da);
    if (len == 0) {
        return EINVAL;
    }
    idx = len - 1;

    if (element) {
        ds_da_get_value(da, idx, element);
    }
    clear_values(da, idx, 1);
    da->lsize--;
    return 0;
}

void ds_da_free(struct dynamic_array *da) {
    if (!da) {
        return;
    }
    free(da->array);
    free(da);
}
