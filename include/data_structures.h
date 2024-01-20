#ifndef __DATA_STRUCTURES_H__
#define __DATA_STRUCTURES_H__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int (*ds_cmp)(void *, void *);

/**
 * @struct dynamic_array
 *
 * Abstract dynamic array.
 */
struct dynamic_array {
    size_t psize; /**< psize is the total space allocated for elements. */
    size_t lsize; /**< lsize is the total utilised capacity. */
    size_t esize; /**< esize is the size in bytes of an element. */
    char *array;  /**< array is the physical array. */
};

/**
 * Allocates a dynamic array. The returned dynamic array should be freed with
 * a call to ds_da_free().
 *
 * @param[in]  esize is the element size of the dynamic array.
 * @param[out] d_da is a pointer to the created dynamic array.
 *
 * @returns 0 on success, otherwise errno-like value.
 */
int ds_da_create(size_t esize, struct dynamic_array **d_da);

/**
 * Get size of a dynamic array.
 *
 * @param[in] da will have its logical size returned.
 *
 * @returns the size of the dynamic array.
 */
size_t ds_da_len(const struct dynamic_array *da);

/**
 * Retrieves a value from the specified dynamic array.
 *
 * @param[in]  da is the dynamic array to access.
 * @param[in]  idx is the position to access the dynamic array.
 * @param[out] element points to the item in the dynamic array.
 *
 * @returns 0 if successful, otherwise errno-like value.
 */
int ds_da_get_value(const struct dynamic_array *da, size_t idx, void *element);

/**
 * Appends an element to the dynamic array.
 *
 * @param[in] da is the dynamic array.
 * @param[in] element will be appended to dynamic array. On failure element
 *            will have zeroes written to it.
 *
 * @returns 0 on success, otherwise errno-like value.
 */
int ds_da_append(struct dynamic_array *da, void *element);

/**
 * Pops an element from the end of the dynamic array.
 *
 * @param[in]  da is a dynamic array.
 * @param[out] element will contain the popped type. On failure, the element
 *             will be unchanged.
 *
 * @returns 0 on success, otherwise errno-like value.
 */
int ds_da_pop(struct dynamic_array *da, void *element);

/**
 * Swaps the two elements in a dynamic array, at the specified indices.
 *
 * @param[in] da is the dynamic array.
 * @param[in] idx1 is the first position in the dynamic array.
 * @param[in] idx2 is the second position in the dynamic array.
 *
 * @returns 0 on success, otherwise errno-like value.
 */
int ds_da_swap(struct dynamic_array *da, size_t idx1, size_t idx2);

/**
 * Free the memory allocated for the dynamic array.
 *
 * @param[in] da is the dynamic array to freed.
 */
void ds_da_free(struct dynamic_array *da);

/**
 * @struct heap
 *
 * Abstract min heap.
 */
struct heap {
    ds_cmp cmp; /**< cmp is the method that takes pointers to the stored
                   elements for comparison. */
    char *element1,
        *element2; /**< block of size esize for passing the value to . */
    struct dynamic_array array; /**< dynamic array to store heap elements. */
};

/**
 * Creates a heap, that should be freed with a call to ds_heap_free().
 *
 * @param[in] esize is the element size stored in the heap.
 * @param[in] cmp_method is a strcmp-like method that operates on two heap
 *            elements, returning less than 0, equal to 0, or greater than
 *            zero if e1 is less than, equal to, or greater than e2.
 * @param[out] d_heap is a pointer to the created heap.
 *
 * @returns 0 on success, otherwise errno-like value.
 */
int ds_heap_create(size_t esize, int (*cmp_method)(void *, void *),
                   struct heap **d_heap);

/**
 * Get the size of a heap.
 *
 * @param[in] heap will have its size.
 *
 * @returns the size of the heap
 */
static inline size_t ds_heap_len(struct heap *heap) {
    return ds_da_len(&heap->array);
}

/**
 * Add an element to the heap.
 *
 * @param[in] heap is the min-heap.
 * @param[in] element will be added to the heap. On failure element
 *            will have zeroes written to it.
 *
 * @returns 0 on success, otherwise errno-like value.
 */
int ds_heap_add(struct heap *heap, void *element);

/**
 * Retrieve the minimum in the heap.
 *
 * @param[in]  heap is an event_heap_e structure.
 * @param[out] element is a pointer to the type to write the min.
 *
 * @returns 0 on success, otherwise errno-like value.
 */
int ds_heap_get_min(struct heap *heap, void *element);

/**
 * Pops the minimum from the heap.
 *
 * @param[in] heap contains the min.
 * @param[out] min will be assigned the popped minimum element, on failure
 *             the element will have zeroes written to it.
 *
 * @returns 0 on success, otherwise errno-like value.
 */
int ds_heap_pop_min(struct heap *heap, void *min);

/**
 * Free the passed heap. Elements are freed with the free_method passed on
 * creation. Accepts NULL.
 *
 * @param[in] heap will be freed.
 */
void ds_heap_free(struct heap *heap);

#endif /* __DATA_STRUCTURES_H__ */
