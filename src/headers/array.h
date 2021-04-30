typedef char elem_t;

enum ReallocType
{   
    DEFAULT_REALLOC,
    LINEAR_REALLOC, 
    EXPONENTIAL_REALLOC,
};

struct Array
{   
    size_t capacity;
    size_t size;
    
    ReallocType realloc_type;
    size_t realloc_factor;

    elem_t* data;
    bool (*comparator)(elem_t* value_a, elem_t* value_b);
};

void constructArray     (Array* array, size_t start_capacity, 
                         ReallocType realloc_type = DEFAULT_REALLOC, size_t realloc_factor = 2, 
                         bool (*comparator)(elem_t* value_a, elem_t* value_b) = nullptr);
void    dumpData        (Array* array);
void    reallocArray    (Array* array, size_t new_capacity = 0);
void    pushBackArray   (Array* array, elem_t value);
void    insertBackArray (Array* array, elem_t* insertable_data, size_t amount);
void    insertArray     (Array* array, size_t idx, elem_t* insertable_data, size_t amount);
void    destructArray   (Array* array);
                                 
void    alignCapacityArrayBySize(Array* array, size_t alignment_size);

int     findArray       (Array* array, elem_t* value);

Array* newArray(size_t start_capacity = 16, 
                ReallocType realloc_type = DEFAULT_REALLOC, size_t realloc_factor = 2,
                bool (*comparator)(elem_t* value_a, elem_t* value_b) = nullptr);
Array*  deleteArray     (Array* array);