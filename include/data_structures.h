#ifndef __DATA_STRUCTURES_DYNAMIC_ARRAY_H__
#define __DATA_STRUCTURES_DYNAMIC_ARRAY_H__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
int ds_da_get_value(struct dynamic_array *da, size_t idx, void *element);

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
 * Free the memory allocated for the dynamic array.
 *
 * @param[in] da is the dynamic array to freed.
 */
void ds_da_free(struct dynamic_array *da);

#endif /* __DATA_STRUCTURES_DYNAMIC_ARRAY_H__ */
