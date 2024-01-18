#include <data_structures.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INITAL_SIZE (1 << 5)
#define GROWTH_FACTOR 1.5f

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
    memset(da->array + (da->psize * da->esize), 0,
           (physical_size - da->psize) * da->esize);
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
    char *pos;

    if (idx >= da->lsize) {
        return EINVAL;
    }

    pos = da->array + (idx * da->esize);
    memcpy(element, pos, da->esize);
    return 0;
}

int ds_da_append(struct dynamic_array *da, void *element) {
    int err = 0;
    size_t idx;

    if (da->lsize >= da->psize) {
        err = ds_da_grow(da);
        if (err != 0) {
            return err;
        }
    }

    idx = da->lsize;
    da->lsize++;
    memcpy(da->array + (idx * da->esize), element, da->esize);
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
    memset(da->array + (idx * da->esize), 0, da->esize);
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
