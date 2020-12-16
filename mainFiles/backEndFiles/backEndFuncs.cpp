#include "../headerFiles/backEndHeader.h"
#include "backEndConstants.h"

#define dbg
#define dmp

#ifndef dbg
#define dbg logWrite( "dbg: (func: %s | line: %d)", __func__, __LINE__); 
#endif

#ifndef dmp
#define dmp logWrite("dmp: (func: %s | line: %d)", __func__, __LINE__); logNode(node);
#endif

//ServiceFuncs
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

inline bool isPrintFunction(String* name)
{   
    dbg;
    return (strcmp(name->string, PRINT) == 0);
}

inline bool isInputFunction(String* name)
{
    return (strcmp(name->string, INPUT) == 0);
}

inline bool isSqrtFunction(String* name)
{
    return (strcmp(name->string, SQRT) == 0);
}

//Parser
Function* getCurrentFunction(Parser* parser)
{
    return parser->table->array[parser->func_id];
}

//NameTable
void constructNameTable(NameTable* table, size_t base_capacity)
{   
    dbg;
    table->array = (Function**)calloc(base_capacity, sizeof(Function*));
    table->capacity = base_capacity;
    table->size  = 0;
    dbg;
}

void reallocateNameTable(NameTable* table)
{
    dbg;
    table->capacity = 2 * table->capacity;
    table->array = (Function**)realloc(table->array, table->capacity * sizeof(Function*));
    dbg;
}

void nameTableAppend(NameTable* table, Function* function)
{
    dbg;
    if (table->size + 1 > table->capacity)
    {
        reallocateNameTable(table);
    }
    table->array[table->size++] = function;
    dbg;
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
    dbg;
    if (node->type != DEFINITION_TYPE) return nullptr;

    NameTable* table = newNameTable(BASE_FUNC_ARRAY_SIZE);

    Function* new_function = nullptr;
    Node* def_node = nullptr;
    while (node != nullptr && 
           node->type == DEFINITION_TYPE)
    {   
        dbg;
        def_node = node->right;
        new_function = newFunction(&def_node->value.naming);
        logWrite("---> find func: %s", new_function->name.string);
        recordArguments(new_function, def_node->right);
        recordLocalVariables(new_function, def_node->left);
        nameTableAppend(table, new_function);
        node = node->left;
        dbg;
    }
    nameTableLog(table);
    dbg;
    return table;
}

void destructNameTable(NameTable* table)
{
    dbg;
    for (size_t i = 0; i < table->size; ++i)
    {
        desctructFunction(table->array[i]);
    }
    free(table->array);
    table->capacity = 0;
    table->size = 0;
    dbg;
}

NameTable* newNameTable(size_t base_capacity)
{   
    dbg;
    NameTable* new_table = (NameTable*)calloc(1, sizeof(NameTable));

    constructNameTable(new_table, base_capacity);
    dbg;
    return new_table;
}

int getFunctionID(NameTable* table, String* name)
{   
    // logWrite("Try to find: %s", name->string);

    for (int i = 0; i < table->size; ++i)
    {   
        // logWrite("Compare with: %s", table->array[i]->name.string);
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
    dbg;
    if (function->variables == nullptr)
    {   
        function->variables = newVariableArray(BASE_VAR_ARRAY_SIZE);
    }
    size_t amount_args = 0;
    while (naming_node != nullptr)
    {
        dbg;
        logWrite("=====>Find new Argument variable: %s", naming_node->value.naming.string);
        variableArrayAppend(function->variables, &naming_node->value.naming);
        naming_node = naming_node->right;
        ++amount_args;
    }
    function->amount_args = amount_args;
    dbg;
}

void recordLocalVariables(Function* function, Node* node)
{   
    dbg;
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
    dbg;
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

inline size_t getAmountVariables(Function* function)
{
    return function->variables->size;
}


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
    dbg;
    if (variables->size + 1 > variables->capacity)
    {
        reallocateVariableArray(variables);
    }
    variables->array[variables->size++] = *variable;
    dbg;
}

void destructVariableArray(VariableArray* variables)
{
    free(variables->array);
    variables->capacity = 0;
    variables->size = 0;
}

VariableArray* newVariableArray(size_t base_capacity)
{
    dbg;
    VariableArray* new_variables = (VariableArray*)calloc(1, sizeof(VariableArray));

    constructVariableArray(new_variables, base_capacity);
    dbg;
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

#define COMPILATOR_ASSERT(node, TYPE) if (node == nullptr || node->type != TYPE)            \
{                                                                                           \
    logWrite("Miss function: %s", __func__);                                                \
    logWrite("Reason: %s", (node == nullptr) ? "Node pointer is nullptr" : "Wrong type");   \
}

#define COMPILATOR_ASSERT_NULLPTR(node) {if (node == nullptr) logWrite("Miss function: %s\nReason: Node pointer is nullptr", __func__);}


//Compilator
void compileMain(Node* node, const char* filename)
{   
    dbg;
    FILE* asm_file = fopen(filename, "w");

    Parser parser = {asm_file, getNameTable(node), 0, 0, 0, 0, 0};

    asmFileWrite(&parser, ";Show must go on!");
    asmFileWrite(&parser, "PUSH 0");
    asmFileWrite(&parser, "POP rbx");
    asmFileWrite(&parser, "PUSH 0");
    asmFileWrite(&parser, "POP [rbx]");
    asmFileWrite(&parser, "CALL %s", MAIN);
    asmFileWrite(&parser, "HLT");

    Node* def_node = node;
    while (def_node != nullptr &&
           def_node->type == DEFINITION_TYPE)
    {   
        dmp;
        compileDefinition(def_node->right, &parser);
        def_node = def_node->left;
        parser.tab_offset = 0;
        asmFileWrite(&parser, "");
        dbg;
    }

    asmFileWrite(&parser, ";Inside my heart is breaking");
    asmFileWrite(&parser, ";My makeup may be flaking");
    asmFileWrite(&parser, ";But my smile, still, stays on");

    fclose(parser.asm_file);
    printf("Compilation finished!\n");
    destructNameTable(parser.table);
    fclose(compilator_logs);
    dbg;
}

void compileDefinition(Node* node, Parser* parser)
{
    dmp;
    parser->func_id = getFunctionID(parser->table, &node->value.naming);

    if (parser->func_id < 0)
    {
        printf("Function did not found.\n");
        return;
    }
    Function* function = getFunction(parser->table, parser->func_id);

    asmFileWrite(parser, ";---Begin compiling Definition: %s---", function->name.string);

    asmFileWrite(parser, "%s: ", function->name.string);

    ++parser->tab_offset;
    
    compileEntry(parser);

    compileVariables(node->right, parser);

    compileBlock(node->left, parser);

    compileExit(parser);

    asmFileWrite(parser, "RET");

    --parser->tab_offset;

    asmFileWrite(parser, ";---End compiling Definition---");
    dbg;
}

/**
* @brief 
* Сдвигает rbx на свободную область памяти. 
* Инициализирует 2 первых ячейки этой области памяти, 
* отвечающих за количество занимаемой памяти и 
* "указатель" на предыдущую область памяти
* @param[in] Node* node 
* @param[in] Parser* parser 
*/
void compileEntry(Parser* parser)
{   
    dbg;
    asmFileWrite(parser, ";---Begin compliling Entry---");
    
    //Что происходит в памяти:
    // |0|1|2|3|4|5|6|7|8|9|10|
    // |______________________|
    // |5|0|_|_|_|6|0|_|_|_|__|
    // ^         ^  
    // |         |
    //rbx  -->  rbx
    // ^         ^
    // |         |
    //before   after

    //|   |      |   |      |   |      |   |      |   |      |   |      |   |
    //|   |      | 5 |      |   |      |   |      |   |      |   |      |   |
    //| 0 | ---> | 0 | ---> | 5 | ---> |   | ---> |   | ---> |   | ---> |   |
    //| 0 |      | 0 |      | 0 |      | 0 |      |   |      | 6 |      |   |
    //|===|      |===|      |===|      |===|      |===|      |===|      |===|
    //rbx = 0                         rbx = 5                    
    asmFileWrite(parser, "PUSH rbx");
    asmFileWrite(parser, "PUSH rbx");
    asmFileWrite(parser, "PUSH [rbx]");
    asmFileWrite(parser, "ADD");
    asmFileWrite(parser, "POP rbx");
    asmFileWrite(parser, "POP [rbx + 1]");
    asmFileWrite(parser, "PUSH %zu", getAmountVariables(getCurrentFunction(parser)) + 2);
    asmFileWrite(parser, "POP [rbx]");

    asmFileWrite(parser, ";---End compliling Entry---");
    dbg;
}

void compileExit(Parser* parser)
{   
    dbg;
    asmFileWrite(parser, ";---Begin compiling Exit---");

    //Что происходит в памяти:
    // |0|1|2|3|4|5|6|7|8|9|10|
    // |______________________|
    // |5|0|_|_|_|6|0|_|_|_|__|
    // ^         ^  
    // |         |
    //rbx  <--  rbx
    // ^         ^
    // |         |
    //after    before

    asmFileWrite(parser, "PUSH [rbx + 1]");
    asmFileWrite(parser, "POP rbx");

    asmFileWrite(parser, ";---End compiling Exit---");    
    dbg;
}

/**
 * @brief (Предполается, что rbx уже сдвинут в область памяти, 
 * где находится функция, для которой инициализируются переменные).
 * Функция инициализирует переменные из стека нашего "процессора" 
 * в наш "стековый фрейм"
 * @param Node* node 
 * @param Parser* parser 
 */
void compileVariables(Node* node, Parser* parser)
{   
    dbg;
    asmFileWrite(parser, ";---Begin compiling Arguments---");

    Function* function = getFunction(parser->table, parser->func_id);
    for (size_t i = 0; i < function->amount_args; ++i)
    {   
        asmFileWrite(parser, ";Init variable: %s", getVariable(function, i)->string);
        asmFileWrite(parser, "POP [rbx + %zu]", i + 2);
    }

    asmFileWrite(parser, ";---End compiling Arguments---");
    dbg;
}

void compileBlock(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT(node, COMPOUND_TYPE);

    Node* statement_node = node->left;

    asmFileWrite(parser, ";---Begin compiling Block---");

    ++parser->tab_offset;

    while (statement_node != nullptr &&
           statement_node->type == STATEMENT_TYPE)
    {   
        compileStatement(statement_node->left, parser);
        statement_node = statement_node->right;
    }

    --parser->tab_offset;

    asmFileWrite(parser, ";---End compiling Block---");
    dbg;
}

void compileStatement(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT_NULLPTR(node);

    asmFileWrite(parser, ";---Begin compiling Statement---");

    switch (node->type)
    {
        case IF_TYPE:
            compileCondition(node, parser);
            break;

        case LOOP_TYPE:
            compileLoop(node, parser);
            break;

        case JUMP_TYPE:
            compileReturn(node, parser);
            break;
        
        case ASSIGN_TYPE:
            compileAssign(node, parser);
            break;

        default:
            if (node->type == MATH_TYPE   ||
                node->type == CALL_TYPE   ||
                node->type == NUMBER_TYPE ||
                node->type == NAMING_TYPE)
            {
                compileExpression(node, parser);
            }
            else COMPILATOR_ASSERT(node, ERROR_TYPE);
            break;
    }

    asmFileWrite(parser, ";---End compiling Statement---");
    dbg;
}

void compileCondition(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT(node, IF_TYPE);

    size_t local_amount_if_else = parser->amount_if_else;
    ++parser->amount_if_else;

    asmFileWrite(parser, ";---Begin compiling Condition---");

    compileExpression(node->left, parser);

    Node* if_else_node = node->right;

    asmFileWrite(parser, "PUSH 0");

    asmFileWrite(parser, "JE IF_END_%zu", local_amount_if_else);

    compileBlock(if_else_node->left, parser);

    asmFileWrite(parser, "JMP IF_ELSE_END_%zu", local_amount_if_else);

    asmFileWrite(parser, "IF_END_%zu:", local_amount_if_else);

    if (if_else_node->right != nullptr) compileBlock(if_else_node->right, parser);

    asmFileWrite(parser, "IF_ELSE_END_%zu: ", local_amount_if_else);

    asmFileWrite(parser, ";---End compiling Condition---");

    dbg;
}

void compileLoop(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT(node, LOOP_TYPE);

    size_t local_amount_loops = parser->amount_loop;
    ++parser->amount_loop;

    asmFileWrite(parser, ";---Begin compiling While---");

    asmFileWrite(parser, "WHILE_%zu:", local_amount_loops);

    ++parser->tab_offset;

    compileExpression(node->left, parser);

    --parser->tab_offset;

    asmFileWrite(parser, "PUSH 0");
    asmFileWrite(parser, "JE WHILE_END_%zu", local_amount_loops);

    compileBlock(node->right, parser);

    asmFileWrite(parser, "JMP WHILE_%zu", local_amount_loops);

    asmFileWrite(parser, "WHILE_END_%zu:", local_amount_loops);

    asmFileWrite(parser, ";---End compiling While---");

    dbg;
}

void compileReturn(Node* node, Parser* parser)
{   
    dmp;

    COMPILATOR_ASSERT(node, JUMP_TYPE);

    asmFileWrite(parser, ";---Begin compiling Return---");

    compileExpression(node->right, parser);

    compileExit(parser);

    asmFileWrite(parser, "RET");

    asmFileWrite(parser, ";---End compiling Return");

    dbg;
}

void compileAssign(Node* node, Parser* parser)
{   
    dmp;

    COMPILATOR_ASSERT(node, ASSIGN_TYPE);

    int var_id = getVariableID(getFunction(parser->table, parser->func_id), 
                                    &node->left->value.naming);
    if (var_id < 0)
    {
        logWrite("Strange Error: Variable was not found in %s", __func__);
        return;
    }
    asmFileWrite(parser, ";---Begin compiling Assign---");

    ++parser->tab_offset;

    compileExpression(node->right, parser);

    asmFileWrite(parser, "POP [rbx + %zu]", var_id + 2);

    asmFileWrite(parser, ";---End compiling Assign---");

    --parser->tab_offset;

    dbg;
}

void compileExpression(Node* node, Parser* parser)
{
    dmp;
    if (node == nullptr) return;

    // asmFileWrite(parser, ";---Begin compiling Expression---");

    switch (node->type)
    {
        case MATH_TYPE:
            compileMath(node, parser);
            break;

        case CALL_TYPE:
            compileCall(node, parser);
            break;

        case NUMBER_TYPE:
            compileNumber(node, parser);
            break;

        case NAMING_TYPE:
            compileVariable(node, parser);
            break;

        default:
            logWrite("Strange error: default condition was reached");
            break;
    }

    // asmFileWrite(parser, ";---End comiling Expression---");
    dbg;
}

void compileMath(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT(node, MATH_TYPE);

    // asmFileWrite(parser, ";---Begin compiling Math---");

    compileExpression(node->left, parser);
    compileExpression(node->right, parser);

    if ((TokenType)node->value.number >= MATH_EQUAL &&
        (TokenType)node->value.number <= MATH_GREATER)
    {
        compileLogicOperators(node, parser);   
        return;    
    }

    switch ((TokenType)node->value.number)
    {
        case MATH_PLUS:
            asmFileWrite(parser, "ADD");
            break;
        
        case MATH_MINUS:
            asmFileWrite(parser, "SUB");
            break;

        case MATH_MUL:
            asmFileWrite(parser, "MUL");
            break;
        
        case MATH_DIV:
            asmFileWrite(parser, "DIV");
            break;
        
        default:
            logWrite("Math operator: default condition was reached");
            break;
    }
    // asmFileWrite(parser, ";---End compiling Math---");
    dbg;
}

void compileCall(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT(node, CALL_TYPE);

    asmFileWrite(parser, ";---Begin compiling Call---");

    ++parser->tab_offset;

    if (isPrintFunction(&node->left->value.naming))
    {
        compilePrint(node, parser);
        --parser->tab_offset;
        return;
    }
    if (isInputFunction(&node->left->value.naming))
    {
        compileInput(node, parser);
        --parser->tab_offset;
        return;
    }
    if (isSqrtFunction(&node->left->value.naming))
    {
        compileSqrt(node, parser);
        --parser->tab_offset;
        return;
    }
    Function* function = getFunction(parser->table, &node->left->value.naming);

    if (function == nullptr)
    {
        printf("Error! Function \"%s\"was not defined!\n", 
                node->left->value.naming.string);
        return;
    }    

    Node* arg_node = node->right;
    while (arg_node != nullptr &&
            arg_node->type == ARG_TYPE)
    {   
        compileExpression(arg_node->left, parser);
        arg_node = arg_node->right;
    }

    asmFileWrite(parser, "CALL %s", function->name.string);

    --parser->tab_offset;

    asmFileWrite(parser, ";---End compiling Call---");
    dbg;

}

void compileNumber(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT(node, NUMBER_TYPE);

    asmFileWrite(parser, ";---Begin pushing Number---");

    asmFileWrite(parser, "PUSH %lg", node->value.number);

    asmFileWrite(parser, ";---End pushing Number");
    dbg;
}

void compileVariable(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT(node, NAMING_TYPE);

    int var_id = getVariableID(getFunction(parser->table, parser->func_id), 
                                &node->value.naming);
    if (var_id < 0)
    {
        printf("Error! Variable \"%s\" is not declared!", 
                node->left->value.naming.string);
        return;
    }
    asmFileWrite(parser, ";--Begin pushing Variable---");

    asmFileWrite(parser, "PUSH [rbx + %zu]", var_id + 2);

    asmFileWrite(parser, ";---End pushing Variable---");
    dbg;
}

void compileLogicOperators(Node* node, Parser* parser)
{   
    dmp;
    COMPILATOR_ASSERT_NULLPTR(node);

    asmFileWrite(parser, ";---Begin compiling LogicOperators---");

    size_t amount_logic_operators = parser->amoutnt_logic;
    ++parser->amoutnt_logic;

    switch ((TokenType)node->value.number)
    {
        case MATH_EQUAL:
            asmFileWrite(parser, "JE LOGIC_%zu", amount_logic_operators);
            break;

        case MATH_EQUAL_LESS:
            asmFileWrite(parser, "JBE LOGIC_%zu", amount_logic_operators);
            break;
        
        case MATH_EQUAL_GREATER:
            asmFileWrite(parser, "JAE LOGIC_%zu", amount_logic_operators);
            break;

        case MATH_LESS:
            asmFileWrite(parser, "JB LOGIC_%zu", amount_logic_operators);
            break;

        case MATH_GREATER:
            asmFileWrite(parser, "JA LOGIC_%zu", amount_logic_operators);
            break;
        
        default:
            logWrite("Logic operator: default condition was reached");
            break;
    }
    ++parser->tab_offset;

    asmFileWrite(parser, "PUSH 0");
    asmFileWrite(parser, "JMP END_LOGIC_%zu", amount_logic_operators);
    asmFileWrite(parser, "LOGIC_%zu: ", amount_logic_operators);
    asmFileWrite(parser, "PUSH 1");

    --parser->tab_offset;

    asmFileWrite(parser, "END_LOGIC_%zu:", amount_logic_operators);

    asmFileWrite(parser, ";---End compiling LogicOperators---");
    dbg;
}

void compilePrint(Node* node, Parser* parser)
{   
    dmp;
    asmFileWrite(parser, ";---Begin compiling Print---");

    Node* arg_node = node->right;
    size_t variables_counter = 0;
    while (arg_node != nullptr &&
            arg_node->type == ARG_TYPE)
    {
        compileExpression(arg_node->left, parser);
        arg_node = arg_node->right;
        ++variables_counter;
    }
    while (variables_counter > 0)
    {
        asmFileWrite(parser, "OUT");
        --variables_counter;
    }

    asmFileWrite(parser, ";---End compiling Print---");
    dbg;
}

void compileInput(Node* node, Parser* parser)
{   
    asmFileWrite(parser, "IN");
}

void compileSqrt(Node* node, Parser* parser)
{
    dmp;
    Node* arg_node = node->right;
    if (arg_node != nullptr &&
            arg_node->type == ARG_TYPE)
    {
        compileExpression(arg_node->left, parser);
        arg_node = arg_node->right;
    }
    asmFileWrite(parser, "SQRT");
    dbg;
}