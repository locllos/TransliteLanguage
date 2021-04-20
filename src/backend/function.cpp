#include "../headers/backEndHeader.h"
#include "constants.h"

//Function
void constructFunction(Function* function, String* name)
{
    function->name = *name;
    function->variables = nullptr;
}

void desctructFunction(Function* function)
{
    destructVariableArray(function->variables);
    function->variables = nullptr;
    destructString(&function->name);   
}

void recordArguments(Function* function, Node* naming_node)
{   
    
    if (function->variables == nullptr)
    {   
        function->variables = newVariableArray(BASE_VAR_ARRAY_SIZE);
    }
    size_t amount_args = 0;
    while (naming_node != nullptr)
    {
        
        logWrite("=====>Find new Argument variable: %s", naming_node->value.naming.string);
        variableArrayAppend(function->variables, &naming_node->value.naming);
        naming_node = naming_node->right;
        ++amount_args;
    }
    function->amount_args = amount_args;
    
}

void recordLocalVariables(Function* function, Node* node)
{   
    
    if (node == nullptr) return;

    if (node->type == ASSIGN_TYPE)
    {
        if (getVariableID(function, &node->left->value.naming) < 0)
        {
            logWrite("===>Find new Local variable: %s", node->left->value.naming.string);
            variableArrayAppend(function->variables, &node->left->value.naming);
        }
    }
    recordLocalVariables(function, node->left);
    recordLocalVariables(function, node->right);
    
}

void functionLog(Function* function)
{
    logWrite("Function: %s", function->name.string);
    for (size_t j = 0; j < function->amount_args; ++j)
    {
        logWrite("\tArgument: %s", function->variables->array[j].string); 
    }
    if (function->amount_args == 0) logWrite("\t(zero amount arguments)");
    logWrite("");
    for (size_t j = function->amount_args; j < function->variables->size; ++j)
    {
        logWrite("\tLocal Variable: %s", function->variables->array[j].string); 
    }
}

Function* newFunction(String* name)
{
    Function* new_function = (Function*)calloc(1, sizeof(Function));

    constructFunction(new_function, name);

    return new_function;
}

String* getVariable(Function* function, size_t idx)
{
    return &function->variables->array[idx];
}

int getVariableID(Function* function, String* variable)
{
    for (size_t i = 0; i < function->variables->size; ++i)
    {
        if (strcmp(getVariable(function, i)->string, variable->string) == 0)
        {
            return i;
        }
    }
    return -1;
}

size_t getAmountVariables(Function* function)
{
    return function->variables->size;
}

Function* getCurrentFunction(Parser* parser)
{
    return parser->table->array[parser->func_id];
}