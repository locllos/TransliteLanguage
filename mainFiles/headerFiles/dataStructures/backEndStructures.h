struct VariableArray
{
    String* array;

    size_t size;
    size_t capacity;
};

struct Function
{
    String name;
    VariableArray* variables;
    size_t amount_args;
};

struct NameTable
{
    size_t size;
    size_t capacity;
    Function** array;
};



struct Parser
{
    FILE* asm_file;
    NameTable* table;

    int func_id;
    int tab_offset;
    
    int amount_if_else;
    int amount_loop;
    int amoutnt_logic;
};