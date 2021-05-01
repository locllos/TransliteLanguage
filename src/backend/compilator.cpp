#include "../headers/backEndHeader.h"
#include "constants.h"

#define COMPILATOR_ASSERT(node, TYPE) if (node == nullptr || node->type != TYPE)            \
{                                                                                           \
    logWrite("Miss function: %s", __func__);                                                \
    logWrite("Reason: %s", (node == nullptr) ? "Node pointer is nullptr" : "Wrong type");   \
}

#define COMPILATOR_ASSERT_NULLPTR(node) {if (node == nullptr) logWrite("Miss function: %s\nReason: Node pointer is nullptr", __func__);}

void initStandartFunctions(Parser* parser)
{   
    pushBackLabelTable(parser->labels, {newString("_print", sizeof("_print") - 1), NOT_INIT, 0, 0, newOffsetArray()});
    pushBackLabelTable(parser->labels, {newString("_scan", sizeof("_scan") - 1), NOT_INIT, 0, 0, newOffsetArray()});
    pushBackLabelTable(parser->labels, {newString("_sqrt", sizeof("_sqrt") - 1), NOT_INIT, 0, 0, newOffsetArray()});

    asmFileWrite(parser, "extern _print");
    asmFileWrite(parser, "extern _scan");
    asmFileWrite(parser, "extern _sqrt");
}

void constructParser(Parser* parser, const char* filename, Node* parsed_tree)
{
    parser->asm_file = fopen(filename, "wb");
    parser->table = getNameTable(parsed_tree);
    parser->text = newArray(DEFAULT_START_CAPACITY);
    parser->labels = newLabelTable(DEFAULT_START_CAPACITY);

    initStandartFunctions(parser);

    parser->amount_if_else = 0;
    parser->amoutnt_logic = 0;
    parser->amount_loop = 0;
    parser->tab_offset = 0;
    parser->func_id = 0;
}

void pushLabel(Parser* parser, String* string, size_t label_offset = NOT_INIT)
{
    pushBackLabelTable(parser->labels, {newString(string), label_offset, 0, 0, newOffsetArray()});
}

void updateLabelOffset(Parser* parser, String* string, size_t label_offset)
{
    updateLabelOffset(parser->labels, string->string, label_offset);
}

Label* putLabel(Parser* parser, String* string, size_t label_offset = NOT_INIT)
{
    return putBackLabelTable(parser->labels, {newString(string), label_offset, 0, 0,  newOffsetArray()});
}

void dumpOffsets(Parser* parser)
{
    size_t amount_labels = parser->labels->size;
    for (size_t label_idx = 0; label_idx < amount_labels; ++label_idx)
    {   
        Label* label = &parser->labels->data[label_idx];

        printf("\n_____\nLabel[=0x%06x]: %s \n", label->label_offset, label->name->string);

        size_t amount_offsets = label->offsets->size;
        for (size_t j = 0; j < amount_offsets; ++j)
        {   
            int32_t offset = (int32_t)label->label_offset - (int32_t)label->offsets->data[j];
            printf("%zu: \t|abs: 0x%06x | rel: 0x%06x|\n", j, label->offsets->data[j], offset);
        }
    }
    printf("\n");
}

void fillOffsets(Parser* parser)
{   
    dumpOffsets(parser);

    size_t amount_labels = parser->labels->size;
    for (size_t label_idx = 0; label_idx < amount_labels; ++label_idx)
    {   
        Label* label = &parser->labels->data[label_idx];
        size_t amount_offsets = label->offsets->size;
        if (label->label_offset == NOT_INIT)
        {   
            if (label->offsets->size != 0) ++parser->amount_externs;
            continue;
        }
        for (size_t j = 0; j < amount_offsets; ++j)
        {   
            int offset = (int)label->label_offset - (int)label->offsets->data[j];

            insertArray(parser->text, label->offsets->data[j] - 3, 
                        (char*)&offset, sizeof(int));            
        }
    }
}

void destructParser(Parser* parser)
{   
    fclose(parser->asm_file);
    
    parser->asm_file = nullptr;
    parser->labels = deleteLabelTable(parser->labels);
    parser->table = deleteNameTable(parser->table);

    if (parser->text != nullptr) parser->text = deleteArray(parser->text);
    parser->amount_if_else = 0;
    parser->amount_loop = 0;
    parser->amoutnt_logic = 0;
    parser->tab_offset = 0;
    parser->func_id = 0;
}

size_t getOffset(Parser* parser)
{
    return parser->text->size - 1;
}

//Compilator
void compileMain(Node* node, const char* filename, const char* elf_filename)
{   
    FILE* asm_file = fopen(filename, "w");

    Parser parser = {};
    constructParser(&parser, filename, node);

    asmFileWrite(&parser, ";Show must go on!");
    asmFileWrite(&parser, "global _start");
    asmFileWrite(&parser, "section .text");

    asmFileWrite(&parser, "_start:");

    ++parser.tab_offset;

    Label* label = putLabel(&parser, newString(MAIN, sizeof(MAIN)));
    asmFileWrite(&parser, CALL, label);
    asmFileWrite(&parser, MOV_REG_CONST64, RAX, CONST64, NONE, 0x3c);
    asmFileWrite(&parser, SYSCALL, VOID_OPERAND, VOID_OPERAND);

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
    asmFileWrite(&parser, ";My make up may be flaking");
    asmFileWrite(&parser, ";But my smile, still, stays on");

    fillOffsets(&parser);

    makeELF64(&parser, filename, elf_filename, START_FUNC_NAME);
    
    destructParser(&parser);

    printf("Compilation finished!\n");
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
    updateLabelOffset(parser, &function->name, getOffset(parser));
    asmFileWrite(parser, "%s: ", function->name.string);

    ++parser->tab_offset;
    
    compileEntry(parser);
    compileBlock(node->left, parser);
    compileExit(parser);

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
    //THIS EXPLANATION IS OLD (for my processorEmulator)
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
    asmFileWrite(parser, PUSH, RBP);
    asmFileWrite(parser, MOV_REG_REG, RBP, RSP);
    //there is initialization local variables  
    asmFileWrite(parser, SUB_CONST32, RSP, CONST32, NONE, 
                 (getAmountVariables(getCurrentFunction(parser))
                  - getCurrentFunction(parser)->amount_args) * gap_size);
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

    asmFileWrite(parser, LEAVE);
    asmFileWrite(parser, RET);

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

    asmFileWrite(parser, XOR, RBX, RBX);

    asmFileWrite(parser, CMP, RAX, RBX);

    Label* if_end = putLabel(parser, newFormattedString("IF_END_%zu", local_amount_if_else));
    asmFileWrite(parser, JE, if_end);

    compileBlock(if_else_node->left, parser);

    Label* if_else_end = putLabel(parser, newFormattedString("IF_ELSE_END_%zu", local_amount_if_else));
    asmFileWrite(parser, JMP, if_else_end);

    asmFileWrite(parser, "IF_END_%zu:", local_amount_if_else);
    setLabelOffset(if_end, getOffset(parser));

    if (if_else_node->right != nullptr) compileBlock(if_else_node->right, parser);

    asmFileWrite(parser, "IF_END_%zu:", local_amount_if_else);
    asmFileWrite(parser, "IF_ELSE_END_%zu: ", local_amount_if_else);
    setLabelOffset(if_else_end, getOffset(parser));

    asmFileWrite(parser, ";[End compiling Condition]");
}

void compileLoop(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, LOOP_TYPE);

    size_t local_amount_loops = parser->amount_loop;
    ++parser->amount_loop;

    asmFileWrite(parser, ";[Begin compiling While]");

    asmFileWrite(parser, "WHILE_%zu:", local_amount_loops);
    Label* while_label = putLabel(parser, newFormattedString("WHILE_%zu", local_amount_loops), getOffset(parser));

    ++parser->tab_offset;

    compileExpression(node->left, parser);

    --parser->tab_offset;

    asmFileWrite(parser, XOR, RBX, RBX);
    asmFileWrite(parser, CMP, RAX, RBX);
    Label* while_end = putLabel(parser, newFormattedString("WHILE_END_%zu", local_amount_loops));
    asmFileWrite(parser, JE, while_end);

    compileBlock(node->right, parser);

    asmFileWrite(parser, JMP, while_label);

    asmFileWrite(parser, "WHILE_END_%zu:", local_amount_loops);
    setLabelOffset(while_end, getOffset(parser));

    asmFileWrite(parser, ";[End compiling While]");
}


void compileReturn(Node* node, Parser* parser)
{   
    COMPILATOR_ASSERT(node, JUMP_TYPE);

    asmFileWrite(parser, ";[Begin compiling Return]");

    compileExpression(node->right, parser);

    compileExit(parser);

    asmFileWrite(parser, RET);

    asmFileWrite(parser, ";[End compiling Return");    
}

void compileAssign(Node* node, Parser* parser)
{   
    COMPILATOR_ASSERT(node, ASSIGN_TYPE);

    Function* function = getFunction(parser->table, parser->func_id);
    int var_id = getVariableID(function, &node->left->value.naming);

    if (var_id < 0)
    {
        logWrite("Strange Error: Variable was not found in %s", __func__);
        return;
    }
    asmFileWrite(parser, ";[Begin compiling Assign]");

    ++parser->tab_offset;

    compileExpression(node->right, parser);

    if (var_id < function->amount_args)
    {
        asmFileWrite(parser, MOV_MEM_REG, RBP, RAX, REG_WITH_OFFSET, 0, (var_id + 2) * gap_size);
    }
    else
    {
        asmFileWrite(parser, MOV_MEM_REG, RBP, RAX, REG_WITH_OFFSET, 0, 
                     (-1) * (var_id - function->amount_args + 1) * gap_size);
    }

    asmFileWrite(parser, ";[End compiling Assign]");

    --parser->tab_offset;   
}

/**
 * @brief result of expression puts into rax
 * 
 * @param node 
 * @param parser 
 */
void compileExpression(Node* node, Parser* parser)
{
    
    if (node == nullptr) return;

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

}

void compileMath(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, MATH_TYPE);


    compileExpression(node->left, parser);
    asmFileWrite(parser, PUSH, RAX);
    compileExpression(node->right, parser);

    asmFileWrite(parser, POP, RBX);

    if ((TokenType)node->value.number >= MATH_EQUAL &&
        (TokenType)node->value.number <= MATH_GREATER)
    {
        compileLogicOperators(node, parser);   
        return;    
    }

    switch ((TokenType)node->value.number)
    {
        case MATH_PLUS:
            asmFileWrite(parser, ADD_REGS, RAX, RBX);
            break;
        
        case MATH_MINUS:
            asmFileWrite(parser, XCHG, RAX, RBX);
            asmFileWrite(parser, SUB_REGS, RAX, RBX);
            break;

        case MATH_MUL:
            asmFileWrite(parser, IMUL, RAX, RBX);
            break;
        
        case MATH_DIV:
            asmFileWrite(parser, XOR, RDX, RDX);
            asmFileWrite(parser, XCHG, RAX, RBX);
            asmFileWrite(parser, IDIV, RBX);
            break;
        
        default:
            logWrite("Math operator: default condition was reached");
            break;
    }
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
        asmFileWrite(parser, PUSH, RAX);
        arg_node = arg_node->right;
    }

    Label* label = getLabel(parser->labels, function->name.string);

    if (label == nullptr) label = putLabel(parser, &function->name);
    
    asmFileWrite(parser, CALL, label);
    asmFileWrite(parser, ADD_CONST32, RSP, CONST32, NONE, 
                 (function->amount_args * gap_size));

    --parser->tab_offset;

    asmFileWrite(parser,";[End compiling Call]");
    

}

void compileNumber(Node* node, Parser* parser)
{   
    COMPILATOR_ASSERT(node, NUMBER_TYPE);

    asmFileWrite(parser, ";[Begin pushing Number]");

    asmFileWrite(parser, MOV_REG_CONST64, RAX, CONST64, NONE, node->value.number);

    asmFileWrite(parser, ";[End pushing Number]");
}

void compileVariable(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT(node, NAMING_TYPE);

    Function* function = getFunction(parser->table, parser->func_id);

    int var_id = getVariableID(function, &node->value.naming);
    if (var_id < 0)
    {
        printf("Error! Variable \"%s\" is not declared!", 
                node->left->value.naming.string);
        return;
    }
    asmFileWrite(parser, ";[Begin pushing Variable: %s]", node->value.naming.string);

    if (var_id < function->amount_args)
    {
        asmFileWrite(parser, MOV_REG_MEM, RAX, RBP, REG_WITH_OFFSET, 0, (var_id + 2) * gap_size);
    }
    else
    {
        asmFileWrite(parser, MOV_REG_MEM, RAX, RBP, REG_WITH_OFFSET, 0, 
                     (-1) * (var_id - function->amount_args + 1) * gap_size);
    }

    asmFileWrite(parser, ";[End pushing Variable: %s]", node->value.naming.string);
    
}

void compileLogicOperators(Node* node, Parser* parser)
{   
    
    COMPILATOR_ASSERT_NULLPTR(node);

    asmFileWrite(parser, ";[Begin compiling LogicOperators]");

    size_t amount_logic_operators = parser->amoutnt_logic;
    ++parser->amoutnt_logic;

    asmFileWrite(parser, CMP, RBX, RAX);

    Label* logic = putLabel(parser,newFormattedString("LOGIC_%zu", amount_logic_operators));
    TokenType logic_operation = (TokenType)node->value.number;
    switch (logic_operation)
    {
        case MATH_EQUAL:
            asmFileWrite(parser, JE, logic);
            break;

        case MATH_EQUAL_LESS:
            asmFileWrite(parser, JLE, logic);
            break;
        
        case MATH_EQUAL_GREATER:
            asmFileWrite(parser, JGE, logic);
            break;

        case MATH_LESS:
            asmFileWrite(parser, JL, logic);
            break;

        case MATH_GREATER:
            asmFileWrite(parser, JG, logic);
            break;
        
        default:
            logWrite("Logic operator: default condition was reached");
            break;
    }
    ++parser->tab_offset;

    asmFileWrite(parser, XOR, RAX, RAX);
    Label* end_logic = putLabel(parser, newFormattedString("END_LOGIC_%zu", amount_logic_operators));
    asmFileWrite(parser, JMP, end_logic);

    asmFileWrite(parser, "LOGIC_%zu: ", amount_logic_operators);
    setLabelOffset(logic, getOffset(parser));

    asmFileWrite(parser, MOV_REG_CONST64, RAX, CONST64, NONE, 1);

    --parser->tab_offset;

    asmFileWrite(parser, "END_LOGIC_%zu:", amount_logic_operators);
    setLabelOffset(end_logic, getOffset(parser));

    asmFileWrite(parser, ";[End compiling LogicOperators]");
}


void compilePrint(Node* node, Parser* parser)
{   
    
    asmFileWrite(parser, ";[Begin compiling Print]");

    Node* arg_node = node->right;
    while (arg_node != nullptr &&
            arg_node->type == ARG_TYPE)
    {   
        compileExpression(arg_node->left, parser);

        asmFileWrite(parser, MOV_REG_REG, RDI, RAX);
        asmFileWrite(parser, CALL, &parser->labels->data[0]);
        arg_node = arg_node->right;
    }

    asmFileWrite(parser, ";[End compiling Print]");
}

void compileInput(Node* node, Parser* parser)
{   
    asmFileWrite(parser, MOV_REG_REG, RAX, RSP);
    asmFileWrite(parser, MOV_REG_CONST64, RBX, CONST64, NONE, 0x10);
    asmFileWrite(parser, XOR, RDX, RDX);
    asmFileWrite(parser, IDIV, RBX);
    asmFileWrite(parser, SUB_REGS, RSP, RDX);

    asmFileWrite(parser, CALL, &parser->labels->data[1]);

    asmFileWrite(parser, ADD_REGS, RSP, RDX);
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
    asmFileWrite(parser, MOV_REG_REG, RDI, RAX);
    asmFileWrite(parser, CALL, &parser->labels->data[2]);
}
