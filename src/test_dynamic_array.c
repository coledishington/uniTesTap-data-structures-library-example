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
    }

    for (int i = 0; i < appends; i++) {
        int element;

        element = -1;
        err = ds_da_get_value(da, i, &element);
        assert(err == 0);
        assert(element == i + 1);
        assert(ds_da_len(da) == appends);
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
    }

    for (int i = 0; i < ARRAY_LEN(appends); i++) {
        int *ptr;

        ptr = NULL;
        err = ds_da_get_value(da, i, &ptr);
        assert(err == 0);
        assert(ptr == &appends[i]);
        assert(*ptr == appends[i]);
        assert(ds_da_len(da) == ARRAY_LEN(appends));
    }

    ds_da_free(da);
    return 0;
}

static int pop_value(void) {
    struct dynamic_array *da;
    int append_value;
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
    for (append_value = 1; append_value < 10; append_value++) {
        err = ds_da_append(da, &append_value);
        assert(err == 0);
        assert(ds_da_len(da) == append_value);

        err = ds_da_pop(da, &pop_value);
        assert(err == 0);
        assert(ds_da_len(da) == append_value - 1);
        assert(append_value == pop_value);

        err = ds_da_append(da, &append_value);
        assert(err == 0);
        assert(ds_da_len(da) == append_value);
    }

    /* Pop down to zero */
    for (append_value = 9; append_value > 0; append_value--) {
        err = ds_da_pop(da, &pop_value);
        assert(err == 0);
        assert(ds_da_len(da) == append_value - 1);
        assert(append_value == pop_value);
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
