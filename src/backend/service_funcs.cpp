#include "../headers/backEndHeader.h"
#include "constants.h"

FILE* compilator_logs = fopen("res/serviceFiles/comp_logs.txt", "w");

void logWrite(const char* format, ...)
{
    va_list begin;
    va_start(begin, format);

    vfprintf(compilator_logs, format, begin);
    fprintf(compilator_logs, "\n");

    va_end(begin);
}

void recordNode(Node* node)
{
    switch (node->type)
    {
        case NAMING_TYPE:
            logWrite("\t(NAMING: [%s])", node->value.naming.string); 
            break;

        case NUMBER_TYPE:
            logWrite("\t(NUMBER: [%lg])", node->value.number); 
            break;

        case MATH_TYPE:
            logWrite("\t(MATH: [%s])", TOKEN[(int)node->value.number].string); 
            break;
        
        default:
            if (node->type >= DEFINITION_TYPE && node->type <= IF_TYPE)
            {
                logWrite("\t(KEY: [%s])", NODE_TYPE[node->type]);
            }
            else
            {
                logWrite("\t{void}");
            }
            break;
    }
}

void logNode(Node* node)
{
    if (node == nullptr)
    {
        logWrite("[\n(nullptr)\n]");
        return;
    }
    logWrite("--------Node-------");

    recordNode(node);
    
    logWrite("\tleft: ");
    if (node->left != nullptr)
    {
        recordNode(node->left);
    }
    else
    {
        logWrite("\tnullptr");
    }
    logWrite("\tright: ");
    if (node->right != nullptr)
    {
        recordNode(node->right);    
    }
    else
    {
        logWrite("\tnullptr");
    }
    
    logWrite("-------------------");
}

/**
 * @brief Записывает ассемблерный код и сама ставит в конце обратный слеш n. 
 * @param[in] Parser* parser 
 * @param[in] const_char*  format 
 * @param[in] ... 
 */
void asmFileWrite(Parser* parser, const char* format, ...)
{   
    va_list begin;
    va_start(begin, format);

    for (size_t i = 0; i < parser->tab_offset; ++i)
    {
        fprintf(parser->asm_file, "\t");
    }

    vfprintf(parser->asm_file, format, begin);
    fprintf(parser->asm_file, "\n");

    va_end(begin);
}

bool isPrintFunction(String* name)
{   
    return (strcmp(name->string, PRINT) == 0);
}

bool isInputFunction(String* name)
{
    return (strcmp(name->string, INPUT) == 0);
}

bool isSqrtFunction(String* name)
{
    return (strcmp(name->string, SQRT) == 0);
}