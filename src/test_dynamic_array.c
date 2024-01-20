#include <assert.h>
#include <data_structures.h>
#include <stddef.h>
#include <stdlib.h>
#include <tap.h>

#define ARRAY_LEN(A) (sizeof(A) / sizeof(*A))

static int create(void) {
    struct dynamic_array *da = NULL;
    int err;

    err = ds_da_create(sizeof(char), &da);
    assert(err == 0);
    assert(da != NULL);
    ds_da_free(da);
    return 0;
}

static int append(void) {
    struct dynamic_array *da;
    const int appends = 25;
    int err;

    err = ds_da_create(sizeof(int), &da);
    assert(err == 0);

    for (int i = 1; i < appends + 1; i++) {
        err = ds_da_append(da, &i);
        assert(err == 0);
        assert(ds_da_len(da) == i);

        /* Validate all elements are still correct */
        for (int idx = 0; idx < i; idx++) {
            int element;

            err = ds_da_get_value(da, idx, &element);
            assert(err == 0);
            assert(element == idx + 1);
        }
    }

    ds_da_free(da);
    return 0;
}

static int append_ref(void) {
    struct dynamic_array *da;
    int appends[25];
    int err;

    err = ds_da_create(sizeof(int *), &da);
    assert(err == 0);

    for (int i = 0; i < ARRAY_LEN(appends); i++) {
        appends[i] = i + 1;
    }

    for (int i = 0; i < ARRAY_LEN(appends); i++) {
        int *ptr;

        ptr = &appends[i];
        err = ds_da_append(da, &ptr);
        assert(err == 0);
        assert(ds_da_len(da) == i + 1);

        /* Validate all elements are still correct */
        for (int j = 0; j <= i; j++) {
            int *ptr;

            ptr = NULL;
            err = ds_da_get_value(da, j, &ptr);
            assert(err == 0);
            assert(ptr == &appends[j]);
            assert(*ptr == appends[j]);
        }
    }

    ds_da_free(da);
    return 0;
}

static int pop_value(void) {
    struct dynamic_array *da;
    int append_value;
    int check_value;
    int append_idx;
    int check_idx;
    int pop_value;
    bool err;

    err = ds_da_create(sizeof(int), &da);
    assert(err == 0);

    /* Test pop with ignored element (NULL) */
    append_value = 465;
    err = ds_da_append(da, &append_value);
    assert(err == 0);
    assert(ds_da_len(da) == 1);
    err = ds_da_pop(da, NULL);
    assert(err == 0);
    assert(ds_da_len(da) == 0);

    /* Insert one and pop */
    append_value = 9;
    ds_da_append(da, &append_value);
    err = ds_da_pop(da, &pop_value);
    assert(err == 0);
    assert(ds_da_len(da) == 0);
    assert(append_value == pop_value);

    /* Many Inserts */
    for (append_idx = 0; append_idx < 10; append_idx++) {
        append_value = append_idx + 1;
        err = ds_da_append(da, &append_value);
        assert(err == 0);
        assert(ds_da_len(da) == append_idx + 1);

        /* Check all values */
        for (check_idx = 0; check_idx <= append_idx; check_idx++) {
            check_value = -1;
            err = ds_da_get_value(da, check_idx, &check_value);
            assert(err == 0);
            assert(check_value == check_idx + 1);
        }

        err = ds_da_pop(da, &pop_value);
        assert(err == 0);
        assert(ds_da_len(da) == append_value - 1);
        assert(append_value == pop_value);

        /* Check all values */
        for (check_idx = 0; check_idx < append_idx; check_idx++) {
            check_value = -1;
            err = ds_da_get_value(da, check_idx, &check_value);
            assert(err == 0);
            assert(check_value == check_idx + 1);
        }

        err = ds_da_append(da, &append_value);
        assert(err == 0);
        assert(ds_da_len(da) == append_value);

        /* Check all values */
        for (check_idx = 0; check_idx <= append_idx; check_idx++) {
            check_value = -1;
            err = ds_da_get_value(da, check_idx, &check_value);
            assert(err == 0);
            assert(check_value == check_idx + 1);
        }
    }

    /* Pop down to zero */
    for (append_idx = 9; append_idx >= 0; append_idx--) {
        err = ds_da_pop(da, &pop_value);
        assert(err == 0);
        assert(ds_da_len(da) == append_idx);
        append_value = append_idx + 1;
        assert(append_value == pop_value);

        /* Check all values */
        for (check_idx = 0; check_idx < append_idx - 1; check_idx++) {
            check_value = -1;
            err = ds_da_get_value(da, check_idx, &check_value);
            assert(err == 0);
            assert(check_value == check_idx + 1);
        }
    }

    /* Pop on zero */
    pop_value = -1;
    assert(ds_da_len(da) == 0);
    err = ds_da_pop(da, &pop_value);
    assert(err != 0);
    assert(ds_da_len(da) == 0);
    /* Value should not be clobbered if an error occurs */
    assert(pop_value == -1);

    ds_da_free(da);
    return 0;
}

int main(void) {
    tap_easy_register(create, NULL);
    tap_easy_register(append, NULL);
    tap_easy_register(append_ref, NULL);
    tap_easy_register(pop_value, NULL);
    tap_easy_runall_and_cleanup();
}
