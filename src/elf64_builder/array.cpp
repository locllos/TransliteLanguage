#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../headers/array.h"

const size_t SYSTEM_CAPACITY = 64;

bool defaultIsEqual(elem_t* value_a, elem_t* value_b)
{
    return (*value_a == *value_b);
}

void constructArray(Array* array, size_t start_capacity, 
                    ReallocType realloc_type, size_t realloc_factor, 
                    bool (*comparator)(elem_t* value_a, elem_t* value_b))
{   
    array->realloc_type = realloc_type;
    array->realloc_factor = realloc_factor;

    array->data = (elem_t*)calloc(start_capacity, sizeof(elem_t));

    array->comparator = comparator;
    array->capacity = start_capacity;
    array->size = 0;
}

void reallocArray(Array* array, size_t new_capacity)
{   
    if (new_capacity == array->capacity && new_capacity != 0) return;

    if (new_capacity == 0)
    {
        switch (array->realloc_type)
        {
            case LINEAR_REALLOC:
                new_capacity = array->capacity + array->realloc_factor;
                break;

            case EXPONENTIAL_REALLOC:
                new_capacity = (array->capacity + 1) * array->realloc_factor;
                break;

            default:
                new_capacity = array->capacity * 2;
        }
    }
    array->data = (elem_t*)realloc(array->data, new_capacity * sizeof(elem_t));
    
    assert(array->data);
    
    if (new_capacity > array->capacity)
    {
        memset(&array->data[array->size], 0x00, (new_capacity - array->capacity) * sizeof(elem_t));  
    }  
    array->capacity = new_capacity;
}

void pushBackArray(Array* array, elem_t value)
{
    insertBackArray(array, &value, 1);
}

void insertBackArray(Array* array, elem_t* insertable_data, size_t amount)
{
    insertArray(array, array->size, insertable_data, amount);
}

void dumpData(Array* array)
{   
    printf("\nData dump: (capacity: %zu | size: %zu)\n", array->capacity, array->size);
    printf("___________________________________\n");
    printf("%08x ", 0x00010203);
    printf("%08x ", 0x04050607);
    printf("%08x ", 0x08090a0b);
    printf("%08x\n", 0x0c0d0e0f);
    printf("___________________________________\n");
    for (size_t i = 0; i < array->size; i+=4)
    {
        if (i != 0 && i % 16 == 0)
        {
            printf("\n");
        }
        printf("%08x ", (int32_t)array->data[i]);
    }
    printf("\n");
}

void insertArray(Array* array, size_t idx, elem_t* insertable_data, size_t amount)
{
    while (idx + amount > array->capacity)
    {
        reallocArray(array);
    }
    memcpy(&array->data[idx], insertable_data, amount * sizeof(elem_t));

    if (amount + idx > array->size)
    {
        array->size = amount + idx;
    }
}

/**
 * @brief Меняет capacity на ближайшее от size число, делимое на alignment_size, 
 * и реалоцирует память по обновлённому capacity
 * @param array 
 * @param alignment_size 
 */
void alignCapacityArrayBySize(Array* array, size_t alignment_size)
{
    size_t increase_factor = 1;
    size_t alignment_capacity = 0;
    size_t total = array->size / alignment_size;
    
    increase_factor = (array->size - total * alignment_size == 0) ? 0 : 1;
    alignment_capacity = (total + increase_factor) * alignment_size; 

    reallocArray(array, alignment_capacity);
    array->capacity = alignment_capacity;
}

void destructArray(Array* array)
{
    free(array->data);

    array->data = nullptr;
    array->capacity = 0;
    array->size = 0;

}

int findArray(Array* array, elem_t* value)
{   
    int size = array->size;
    bool (*comparator)(elem_t*, elem_t*) = array->comparator == nullptr ? defaultIsEqual : array->comparator;

    for (int i = 0; i < size; ++i)
    {   
        if (comparator(array->data + i, value))
        {
            return i;
        }
    }
    return -1;
}

Array* newArray(size_t start_capacity, 
                ReallocType realloc_type, size_t realloc_factor,
                bool (*comparator)(elem_t* value_a, elem_t* value_b))
{
    Array* array = (Array*)calloc(1, sizeof(Array));

    constructArray(array, start_capacity, realloc_type, realloc_factor, comparator);

    return array;
}

Array* deleteArray(Array* array)
{
    destructArray(array);
    free(array);

    return nullptr;
}