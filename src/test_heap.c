#include <assert.h>
#include <data_structures.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <tap.h>

#define ARRAY_LEN(A) (sizeof(A) / sizeof(*A))

static int strcmp_void(void *void1, void *void2) {
    const char *s1 = *((char **)void1);
    const char *s2 = *((char **)void2);
    return strcmp(s1, s2);
}

static int intcmp(void *v1, void *v2) {
    int *i1 = v1;
    int *i2 = v2;
    return *i1 - *i2;
}

static int get_min(int m1, int m2) { return intcmp(&m1, &m2) <= 0 ? m1 : m2; }

static int create(void) {
    struct heap *heap;
    int err;

    err = ds_heap_create(sizeof(int), intcmp, &heap);
    assert(err == 0);
    assert(heap);
    ds_heap_free(heap);
    return 0;
}

static int add(void) {
    char *elements[] = {"abc", "bca", "cab"};
    char *element = NULL;
    struct heap *heap;
    int err;

    err = ds_heap_create(sizeof(char *), strcmp_void, &heap);
    assert(err == 0);

    err = ds_heap_add(heap, &elements[1]);
    assert(err == 0);
    err = ds_heap_get_min(heap, &element);
    assert(err == 0);
    assert(element == elements[1]);

    err = ds_heap_add(heap, &elements[0]);
    assert(err == 0);
    err = ds_heap_get_min(heap, &element);
    assert(err == 0);
    assert(element == elements[0]);

    err = ds_heap_add(heap, &elements[2]);
    assert(err == 0);
    err = ds_heap_get_min(heap, &element);
    assert(err == 0);
    assert(element == elements[0]);

    ds_heap_free(heap);
    return 0;
}

static int pop(void) {
    int elements[] = {9, 7, 8, 4, 5, 6, 3, 1, 2, 0};
    struct heap *heap;
    int check_element;
    int pop_min;
    int element;
    int min;
    int err;

    err = ds_heap_create(sizeof(int), intcmp, &heap);
    assert(err == 0);

    min = elements[0];
    for (int i = 0; i < ARRAY_LEN(elements); i++) {
        element = elements[i];
        min = get_min(element, min);

        err = ds_heap_add(heap, &element);
        assert(err == 0);

        err = ds_heap_get_min(heap, &check_element);
        assert(err == 0);
        assert(check_element == min);
    }
    err = ds_heap_get_min(heap, &check_element);
    assert(err == 0);
    assert(check_element == 0);

    for (int i = 0; i < ARRAY_LEN(elements); i++) {
        err = ds_heap_pop_min(heap, &pop_min);
        assert(err == 0);
        assert(pop_min == i);

        if (i == ARRAY_LEN(elements) - 1) {
            continue;
        }
        err = ds_heap_get_min(heap, &check_element);
        assert(err == 0);
        assert(check_element == i + 1);
    }

    ds_heap_free(heap);
    return 0;
}

int main(void) {
    tap_easy_register(create, "Checks creation");
    tap_easy_register(add, "Checks adding values");
    tap_easy_register(pop, "Checks popping min");
    tap_easy_runall_and_cleanup();
}
