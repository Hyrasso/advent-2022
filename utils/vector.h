#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct 
{
    int *array;
    size_t size;
    size_t capacity;
} vector;

/// @brief Create a vector of int
/// @param initial_capacity 
/// @return NULL if could not create the array
vector* vec_create(size_t initial_capacity);
/// @brief Initialise a vector with given capacity
/// @param vec 
/// @param initial_capacity 
/// @return NULL if could not allocate initial array
vector* vec_init(vector* vec, size_t initial_capacity);
/// @brief Set the vector capacity
/// @param vec 
/// @param new_capacity 
/// @return -1 if resize failed
int vec_resize(vector* vec, size_t new_capacity);
/// @brief Add a new value at the end of the array, resizing the array if necessary
/// @param vec 
/// @param value 
void vec_append(vector* vec, int value);
/// @brief Release memory allocated by vec
/// @param vec 
void vec_free(vector vec);
/// @brief Print debug info for a vector
/// @param vec 
void vec_debug(vector* vec);
/// @brief Sort the array inplace, in ascending order
/// @param vec 
void vec_bubble_sort(vector* vec);

#endif