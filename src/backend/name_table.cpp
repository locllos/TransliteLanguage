#include "../headers/backEndHeader.h"
#include "constants.h"

//NameTable
void constructNameTable(NameTable* table, size_t base_capacity)
{   
    table->array = (Function**)calloc(base_capacity, sizeof(Function*));
    table->capacity = base_capacity;
    table->size  = 0;
}

void reallocateNameTable(NameTable* table)
{
    table->capacity = 2 * table->capacity;
    table->array = (Function**)realloc(table->array, table->capacity * sizeof(Function*));
}

void nameTableAppend(NameTable* table, Function* function)
{
    if (table->size + 1 > table->capacity)
    {
        reallocateNameTable(table);
    }
    table->array[table->size++] = function;
}

void nameTableLog(NameTable* table)
{
    logWrite("===\nNameTable[%d]: ", table->size);

    for (size_t i = 0; i < table->size; ++i)
    {
        logWrite("\tFunction: %s", table->array[i]->name.string);
        for (size_t j = 0; j < table->array[i]->amount_args; ++j)
        {
            logWrite("\t\tArgument: %s", table->array[i]->variables->array[j].string); //Я снова упоролся
        }
        if (table->array[i]->amount_args == 0) logWrite("\t\t(zero amount arguments)");
        logWrite("");
        for (size_t j = table->array[i]->amount_args; j < table->array[i]->variables->size; ++j)
        {
            logWrite("\t\tLocal Variable: %s", table->array[i]->variables->array[j].string); //Я снова упоролся
        }
    }
}

NameTable* getNameTable(Node* node)
{
    if (node->type != DEFINITION_TYPE) return nullptr;

    NameTable* table = newNameTable(BASE_FUNC_ARRAY_SIZE);

    Function* new_function = nullptr;
    Node* def_node = nullptr;
    while (node != nullptr && 
           node->type == DEFINITION_TYPE)
    {          
        def_node = node->right;
        new_function = newFunction(&def_node->value.naming);
        logWrite("---> find func: %s", new_function->name.string);
        recordArguments(new_function, def_node->right);
        recordLocalVariables(new_function, def_node->left);
        nameTableAppend(table, new_function);
        node = node->left;
        
    }
    nameTableLog(table);
    
    return table;
}

void destructNameTable(NameTable* table)
{
    for (size_t i = 0; i < table->size; ++i)
    {
        desctructFunction(table->array[i]);
    }
    free(table->array);
    table->capacity = 0;
    table->size = 0;
}

NameTable* newNameTable(size_t base_capacity)
{      
    NameTable* new_table = (NameTable*)calloc(1, sizeof(NameTable));

    constructNameTable(new_table, base_capacity);
    
    return new_table;
}

int getFunctionID(NameTable* table, String* name)
{   
    for (int i = 0; i < table->size; ++i)
    {   
        if (strcmp(table->array[i]->name.string, name->string) == 0)
        {
            return i;
        }
    }
    return -1;
}

Function* getFunction(NameTable* table, String* name)
{
    int func_id = getFunctionID(table, name);

    return (func_id > -1) ? table->array[func_id] : nullptr; 
}

Function* getFunction(NameTable* table, size_t func_id)
{
    return table->array[func_id];
}