#include "../headers/backEndHeader.h"
#include "constants.h"

#define COMPILATOR_ASSERT(node, TYPE) if (node == nullptr || node->type != TYPE)            \
{                                                                                           \
    logWrite("Miss function: %s", __func__);                                                \
    logWrite("Reason: %s", (node == nullptr) ? "Node pointer is nullptr" : "Wrong type");   \
}

#define COMPILATOR_ASSERT_NULLPTR(node) {if (node == nullptr) logWrite("Miss function: %s\nReason: Node pointer is nullptr", __func__);}

//Compilator
void compileMain(Node* node, const char* filename)
{   
    FILE* asm_file = fopen(filename, "w");

    Parser parser = {asm_file, getNameTable(node), 0, 0, 0, 0, 0};

    asmFileWrite(&parser, ";Show must go on!");
    asmFileWrite(&parser, "global _start");
    asmFileWrite(&parser, "section .text");
    // asmFileWrite(parser, "PUSH 0");
    // asmFileWrite(parser, "POP rbx");
    // asmFileWrite(parser, "PUSH 0");
    // asmFileWrite(parser, "POP [rbx]");
    asmFileWrite(&parser, "_start:");

    ++parser.tab_offset;

    asmFileWrite(&parser, "call %s", MAIN);
    asmFileWrite(&parser, "mov rax, 3ch");
    asmFileWrite(&parser, "syscall");
    // asmFileWrite(parser, "HLT");

    Node* def_node = node;
    while (def_node != nullptr &&
           def_node->type == DEFINITION_TYPE)
    {   
        
        compileDefinition(def_node->right, &parser);
        def_node = def_node->left;
        parser.tab_offset = 0;
        asmFileWrite(&parser, "");
        
    }
    asmFileWrite(&parser, ";Inside my heart is breaking");
    asmFileWrite(&parser, ";My makeup may be flaking");
    asmFileWrite(&parser, ";But my smile, still, stays on");

    fclose(parser.asm_file);
    printf("Compilation finished!\n");
    destructNameTable(parser.table);
    fclose(compilator_logs);
    
}

void compileDefinition(Node* node, Parser* parser)
{
    
    parser->func_id = getFunctionID(parser->table, &node->value.naming);

    if (parser->func_id < 0)
    {
        printf("Function did not found.\n");
        return;
    }
    Function* function = getFunction(parser->table, parser->func_id);

    asmFileWrite(parser, ";[Begin compiling Definition: %s]", function->name.string);
    asmFileWrite(parser, "%s: ", function->name.string);

    ++parser->tab_offset;
    
    compileEntry(parser);
    // compileVariables(node->right, parser);
    compileBlock(node->left, parser);
    compileExit(parser);

    // asmFileWrite(parser, "RET");

    --parser->tab_offset;

    asmFileWrite(parser, ";[End compiling Definition]");
    
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
    
    asmFileWrite(parser, ";[Begin compiling Entry]");
    //ATTENTION!
    //THIS EXPLANATION IS OLD
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
    
    //NEW VERSION USES CDECL STYLE                 
    asmFileWrite(parser, "push rbp");
    asmFileWrite(parser, "mov rbp, rsp");
    asmFileWrite(parser, "sub rsp, 8 * %zu", getAmountVariables(getCurrentFunction(parser))
                                             - getCurrentFunction(parser)->amount_args); //<--there is initialization local variables  

    asmFileWrite(parser, ";[End compiling Entry]");
    
}

void compileExit(Parser* parser)
{   
    
    asmFileWrite(parser, ";[Begin compiling Exit]");

    //ATTENTION!
    //THIS EXPLANATION IS OLD
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

    //NEW VERSION USES CDECL STYLE                 
    // asmFileWrite(parser, "PUSH [rbx + 1]");
    // asmFileWrite(parser, "POP rbx");

    asmFileWrite(parser, "leave"); // it's equal to mov rsp, rbp && pop rbp
    asmFileWrite(parser, "ret");

    asmFileWrite(parser, ";[End compiling Exit]");    
    
}

/** OLD VERSION, WHICH DOESN'T USE
 * @brief (Предполается, что rbx уже сдвинут в область памяти, 
 * где находится функция, для которой инициализируются переменные).
 * Функция инициализирует переменные из стека нашего "процессора" 
 * в наш "стековый фрейм"
 * @param Node* node 
 * @param Parser* parser 
 */
void compileVariables(Node* node, Parser* parser)
{   
    
    asmFileWrite(parser, ";[Begin compiling Arguments]");

    Function* function = getFunction(parser->table, parser->func_id);
    for (size_t i = 0; i < function->amount_args; ++i)
    {   
        asmFileWrite(parser, ";Init variable: %s", getVariable(function, i)->string);
        asmFileWrite(parser, "POP [rbx + %zu]", i + 2);
    }

    asmFileWrite(parser, ";[End compiling Arguments]");
    
}

void compileBlock(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, COMPOUND_TYPE);

    Node* statement_node = node->left;

    asmFileWrite(parser, ";[Begin compiling Block]");

    ++parser->tab_offset;

    while (statement_node != nullptr &&
           statement_node->type == STATEMENT_TYPE)
    {   
        compileStatement(statement_node->left, parser);
        statement_node = statement_node->right;
    }

    --parser->tab_offset;

    asmFileWrite(parser, ";[End compiling Block]");
    
}

void compileStatement(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT_NULLPTR(node);

    asmFileWrite(parser, ";[Begin compiling Statement]");

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

    asmFileWrite(parser, ";[End compiling Statement]");
    
}

void compileCondition(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, IF_TYPE);

    size_t local_amount_if_else = parser->amount_if_else;
    ++parser->amount_if_else;

    asmFileWrite(parser, ";[Begin compiling Condition]");

    compileExpression(node->left, parser);

    Node* if_else_node = node->right;

    // asmFileWrite(parser, "PUSH 0");
    asmFileWrite(parser, "xor rbx, rbx");

    // asmFileWrite(parser, "JE IF_END_%zu", local_amount_if_else);
    asmFileWrite(parser, "cmp rax, rbx");
    asmFileWrite(parser, "je IF_END_%zu", local_amount_if_else);

    compileBlock(if_else_node->left, parser);

    // asmFileWrite(parser, "JMP IF_ELSE_END_%zu", local_amount_if_else);
    asmFileWrite(parser, "jmp IF_ELSE_END_%zu", local_amount_if_else);

    asmFileWrite(parser, "IF_END_%zu:", local_amount_if_else);

    if (if_else_node->right != nullptr) compileBlock(if_else_node->right, parser);

    asmFileWrite(parser, "IF_ELSE_END_%zu: ", local_amount_if_else);

    asmFileWrite(parser, ";[End compiling Condition]");
}

void compileLoop(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, LOOP_TYPE);

    size_t local_amount_loops = parser->amount_loop;
    ++parser->amount_loop;

    asmFileWrite(parser, ";[Begin compiling While]");

    asmFileWrite(parser, "WHILE_%zu:", local_amount_loops);

    ++parser->tab_offset;

    compileExpression(node->left, parser);

    --parser->tab_offset;

    // asmFileWrite(parser, "PUSH 0");
    asmFileWrite(parser, "xor rbx, rbx");
    asmFileWrite(parser, "cmp rax, rbx");
    asmFileWrite(parser, "je WHILE_END_%zu", local_amount_loops);

    compileBlock(node->right, parser);

    asmFileWrite(parser, "jmp WHILE_%zu", local_amount_loops);

    asmFileWrite(parser, "WHILE_END_%zu:", local_amount_loops);

    asmFileWrite(parser, ";[End compiling While]");
}

void compileReturn(Node* node, Parser* parser)
{   
    COMPILATOR_ASSERT(node, JUMP_TYPE);

    asmFileWrite(parser, ";[Begin compiling Return]");

    compileExpression(node->right, parser);

    compileExit(parser);

    // asmFileWrite(parser, "ret");

    asmFileWrite(parser, ";[End compiling Return");

    
}

void compileAssign(Node* node, Parser* parser)
{   
    COMPILATOR_ASSERT(node, ASSIGN_TYPE);

    char operation = '-';
    int service_offset = 1;
    Function* function = getFunction(parser->table, parser->func_id);
    int var_id = getVariableID(function, &node->left->value.naming);

    if (var_id < 0)
    {
        logWrite("Strange Error: Variable was not found in %s", __func__);
        return;
    }
    if (var_id < function->amount_args) 
    {
        operation = '+';
        service_offset = 2;
    }

    asmFileWrite(parser, ";[Begin compiling Assign]");

    ++parser->tab_offset;

    compileExpression(node->right, parser);

    asmFileWrite(parser, "mov [rbp %c %zu], rax", operation, (var_id + service_offset) * gap_size);

    asmFileWrite(parser, ";[End compiling Assign]");

    --parser->tab_offset;   
}

/**
 * @brief result of expression is put into rax
 * 
 * @param node 
 * @param parser 
 */
void compileExpression(Node* node, Parser* parser)
{
    
    if (node == nullptr) return;

    // asmFileWrite(parser, ";[Begin compiling Expression]");

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

    // asmFileWrite(parser, ";[End compiling Expression]");
    
}

void compileMath(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, MATH_TYPE);

    // asmFileWrite(parser, ";[Begin compiling Math]");

    compileExpression(node->left, parser);
    asmFileWrite(parser, "push rax");
    compileExpression(node->right, parser);

    asmFileWrite(parser, "pop rbx");

    if ((TokenType)node->value.number >= MATH_EQUAL &&
        (TokenType)node->value.number <= MATH_GREATER)
    {
        compileLogicOperators(node, parser);   
        return;    
    }

    switch ((TokenType)node->value.number)
    {
        case MATH_PLUS:
            asmFileWrite(parser, "add rax, rbx");
            break;
        
        case MATH_MINUS:
            asmFileWrite(parser, "xchg rax, rbx");
            asmFileWrite(parser, "sub rax, rbx");
            break;

        case MATH_MUL:
            asmFileWrite(parser, "imul rax, rbx");
            asmFileWrite(parser, "xor rdx, rdx");

            break;
        
        case MATH_DIV:
            asmFileWrite(parser, "xor rdx, rdx");
            asmFileWrite(parser, "xchg rax, rbx");
            asmFileWrite(parser, "idiv rbx");
            break;
        
        default:
            logWrite("Math operator: default condition was reached");
            break;
    }
    // asmFileWrite(parser, ";[End compiling Math]");
    
}

void compileCall(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, CALL_TYPE);

    asmFileWrite(parser, ";[Begin compiling Call]");

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
        asmFileWrite(parser, "push rax");
        arg_node = arg_node->right;
    }

    asmFileWrite(parser, "call %s", function->name.string);
    asmFileWrite(parser, "add rsp, %zu", getAmountVariables(function) * gap_size);

    --parser->tab_offset;

    asmFileWrite(parser,";[End compiling Call]");
    

}

void compileNumber(Node* node, Parser* parser)
{   
    COMPILATOR_ASSERT(node, NUMBER_TYPE);

    asmFileWrite(parser, ";[Begin pushing Number]");

    asmFileWrite(parser, "mov rax, %lld", node->value.number);

    asmFileWrite(parser, ";[End pushing Number]");
}

void compileVariable(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, NAMING_TYPE);

    char operation = '-';
    int service_offset = 1;
    Function* function = getFunction(parser->table, parser->func_id);

    int var_id = getVariableID(function, &node->value.naming);
    if (var_id < 0)
    {
        printf("Error! Variable \"%s\" is not declared!", 
                node->left->value.naming.string);
        return;
    }
    if (var_id < function->amount_args)
    {
        service_offset = 2;
        operation = '+';
    }
    asmFileWrite(parser, ";[Begin pushing Variable: %s]", node->value.naming.string);

    asmFileWrite(parser, "mov rax, [rbp %c %zu]", operation, (var_id + service_offset) * gap_size);

    asmFileWrite(parser, ";[End pushing Variable: %s]", node->value.naming.string);
    
}

void compileLogicOperators(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT_NULLPTR(node);

    asmFileWrite(parser, ";[Begin compiling LogicOperators]");

    size_t amount_logic_operators = parser->amoutnt_logic;
    ++parser->amoutnt_logic;

    asmFileWrite(parser, "cmp rbx, rax");

    switch ((TokenType)node->value.number)
    {
        case MATH_EQUAL:
            asmFileWrite(parser, "je LOGIC_%zu", amount_logic_operators);
            break;

        case MATH_EQUAL_LESS:
            asmFileWrite(parser, "jbe LOGIC_%zu", amount_logic_operators);
            break;
        
        case MATH_EQUAL_GREATER:
            asmFileWrite(parser, "jae LOGIC_%zu", amount_logic_operators);
            break;

        case MATH_LESS:
            asmFileWrite(parser, "jb LOGIC_%zu", amount_logic_operators);
            break;

        case MATH_GREATER:
            asmFileWrite(parser, "ja LOGIC_%zu", amount_logic_operators);
            break;
        
        default:
            logWrite("Logic operator: default condition was reached");
            break;
    }
    ++parser->tab_offset;

    asmFileWrite(parser, "xor rax, rax");
    asmFileWrite(parser, "jmp END_LOGIC_%zu", amount_logic_operators);
    asmFileWrite(parser, "LOGIC_%zu: ", amount_logic_operators);
    asmFileWrite(parser, "mov rax, 1");

    --parser->tab_offset;

    asmFileWrite(parser, "END_LOGIC_%zu:", amount_logic_operators);

    asmFileWrite(parser, ";[End compiling LogicOperators]");
    
}

/**
 * @brief TODO
 * 
 * @param node 
 * @param parser 
 */
void compilePrint(Node* node, Parser* parser)
{   
    
    asmFileWrite(parser, ";[Begin compiling Print]");

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

    asmFileWrite(parser, ";[End compiling Print]");
    
}

/**
 * @brief TODO
 * 
 * @param node 
 * @param parser 
 */
void compileInput(Node* node, Parser* parser)
{   
    asmFileWrite(parser, "IN");
}

void compileSqrt(Node* node, Parser* parser)
{
    
    Node* arg_node = node->right;
    if (arg_node != nullptr &&
            arg_node->type == ARG_TYPE)
    {
        compileExpression(arg_node->left, parser);
        arg_node = arg_node->right;
    }
    asmFileWrite(parser, "cvtsi2sd xmm0, rax");
    asmFileWrite(parser, "sqrtsd xmm0, xmm0");
    asmFileWrite(parser, "cvtsd2si rax, xmm0");
}

#undef put