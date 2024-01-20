#include <data_structures.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

#include "internal.h"

static size_t ds_heap_find_parent_index(size_t cindex) {
    return (size_t)floor((cindex) / 2);
}

static int ds_heap_min_child(const struct heap *heap, size_t pindex,
                             size_t *cindex) {
    size_t cindex1, cindex2;

    cindex1 = 2 * pindex + 1;
    if (ds_da_len(&heap->array) <= cindex1) {
        return EINVAL;
    }

    cindex2 = 2 * pindex + 2;
    if (ds_da_len(&heap->array) <= cindex2) {
        *cindex = cindex1;
        return 0;
    }

    ds_da_get_value(&heap->array, cindex1, heap->element1);
    ds_da_get_value(&heap->array, cindex2, heap->element2);
    if (heap->cmp(heap->element1, heap->element2) < 0) {
        *cindex = cindex1;
    } else {
        *cindex = cindex2;
    }
    return 0;
}

static size_t ds_heap_bubble_up(struct heap *heap, size_t cindex) {
    size_t pindex;

    if (cindex == 0) {
        return cindex;
    }

    pindex = ds_heap_find_parent_index(cindex);
    ds_da_get_value(&heap->array, cindex, heap->element1);
    ds_da_get_value(&heap->array, pindex, heap->element2);
    if (heap->cmp(heap->element1, heap->element2) >= 0) {
        return cindex;
    }

    ds_da_swap(&heap->array, cindex, pindex);
    return ds_heap_bubble_up(heap, pindex);
}

static size_t ds_heap_bubble_down(struct heap *heap, size_t pindex) {
    size_t cindex;

    if (ds_heap_min_child(heap, pindex, &cindex) != 0) {
        return pindex;
    }

    ds_da_get_value(&heap->array, pindex, heap->element1);
    ds_da_get_value(&heap->array, cindex, heap->element2);
    if (heap->cmp(heap->element1, heap->element2) <= 0) {
        return pindex;
    }

    ds_da_swap(&heap->array, pindex, cindex);
    return ds_heap_bubble_down(heap, cindex);
}

int ds_heap_create(size_t esize, int (*cmp_method)(void *, void *),
                   struct heap **d_heap) {
    struct heap *heap;
    int err;

    heap = malloc(sizeof(*heap));
    if (!heap) {
        return errno;
    }

    heap->element1 = malloc(esize);
    if (!heap->element1) {
        err = errno;
        free(heap);
        return err;
    }

    heap->element2 = malloc(esize);
    if (!heap->element2) {
        err = errno;
        free(heap->element1);
        free(heap);
        return err;
    }

    err = ds_da_init(esize, &heap->array);
    if (err != 0) {
        free(heap);
        return err;
    }

    heap->cmp = cmp_method;
    *d_heap = heap;
    return 0;
}

int ds_heap_add(struct heap *heap, void *element) {
    int err;

    err = ds_da_append(&heap->array, element);
    if (err != 0) {
        return err;
    }

    ds_heap_bubble_up(heap, ds_da_len(&heap->array) - 1);
    return 0;
}

int ds_heap_get_min(struct heap *heap, void *element) {
    return ds_da_get_value(&heap->array, 0, element);
}

int ds_heap_pop_min(struct heap *heap, void *min) {
    int err;

    if (ds_da_len(&heap->array) > 0) {
        ds_da_swap(&heap->array, 0, ds_da_len(&heap->array) - 1);
    }

    /* Pop will return false if out of range */
    err = ds_da_pop(&heap->array, min);
    if (err != 0) {
        return err;
    }

    if (ds_da_len(&heap->array) > 0) {
        ds_heap_bubble_down(heap, 0);
    }
    return 0;
}

void ds_heap_free(struct heap *heap) {
    if (heap) {
        free(heap->array.array);
        free(heap->element1);
        free(heap->element2);
        free(heap);
    }
}
