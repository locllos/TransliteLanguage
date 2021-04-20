#include "../headers/backEndHeader.h"
#include "constants.h"

//VariableArray
void constructVariableArray(VariableArray* variables, size_t base_capacity)
{
    variables->array = (String*)calloc(base_capacity, sizeof(String));
    variables->capacity = base_capacity;
    variables->size = 0;
}

void reallocateVariableArray(VariableArray* variables)
{
    variables->capacity = 2 * variables->capacity;
    variables->array = (String*)realloc(variables->array, variables->capacity * sizeof(String));
}

void variableArrayAppend(VariableArray* variables, String* variable)
{   
    
    if (variables->size + 1 > variables->capacity)
    {
        reallocateVariableArray(variables);
    }
    variables->array[variables->size++] = *variable;
    
}

void destructVariableArray(VariableArray* variables)
{
    free(variables->array);
    variables->capacity = 0;
    variables->size = 0;
}

VariableArray* newVariableArray(size_t base_capacity)
{
    
    VariableArray* new_variables = (VariableArray*)calloc(1, sizeof(VariableArray));

    constructVariableArray(new_variables, base_capacity);
    
    return new_variables;
}

int getVariableOffset(VariableArray* variables, String* variable)
{
    for (size_t i = 0; i < variables->size; ++i)
    {
        if (strcmp(variables->array[i].string, variable->string) == 0)
        {
            return i;
        }
    }
    return -1;
}
