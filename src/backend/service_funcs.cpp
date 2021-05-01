#include "../headers/backEndHeader.h"
#include "constants.h"

FILE* compilator_logs = fopen("res/serviceFiles/comp_logs.txt", "w");

size_t getUpperMultiple(size_t number, size_t divisor)
{
    size_t increase_factor = 1;
    size_t upper_multiple_number = 0;
    size_t total = number / divisor;
    
    increase_factor = (number - total * divisor == 0) ? 0 : 1;

    return (total + increase_factor) * divisor; 
}

void swapOperands(OperandType* operand_a, OperandType* operand_b)
{
    OperandType save_operand = *operand_a;
    *operand_a = *operand_b;
    *operand_b = save_operand; 
}

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
    if (parser->asm_file == nullptr) return;

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

bool atLeastOneOperandRBPorR13(MovType mov_type, OperandType operand_a, OperandType operand_b)
{
    return ((operand_a == RSP || operand_a == RBP ||
            operand_a == R12 || operand_a == RSP) && (mov_type == MOV_MEM_REG)
            ||
            (operand_b == RSP || operand_b == RBP ||
            operand_b == R12 || operand_b == RSP) && (mov_type == MOV_REG_MEM));
} 

bool atLeastOneOperandRSPorR12(MovType mov_type, OperandType operand_a, OperandType operand_b)
{
    return ((operand_a == RSP || operand_a == R12) && (mov_type == MOV_MEM_REG) || 
            (operand_b == RSP || operand_b == R12) && (mov_type == MOV_REG_MEM));
}

bool isNewRegister(OperandType operand)
{
    return (operand > RDI && operand < AMOUNT_REGS);
}

bool isRegister(OperandType operand)
{
    return (operand >= RAX && operand < AMOUNT_REGS);
}

// Description for mov <r/m>, <r/m>, and for other instruction behaviour is similar
//
//              REX  3   2   1   0       OPCODE                     MODRM                                 
// +---+---+---+---+---+---+---+---+ +---------------+ +-----------+-----------------+----------------+ +------------------------+
// | 0   1   0   0 | W | R | X | B | | 1 byte opcode | | 2-bit mod | 3-bit reg field | 3-bit rm field | | 8-bit(32-bit) constant |
// +---+---+---+---+---+---+---+---+ +---------------+ +-----------+-----------------+----------------+ +------------------------+
// W - 64-bit register using
// R - extended reg field (r8-r15 using) 
// X = 0, we dont use this feature
// B - extended rm field (r8-r15 using)
// mod:
//    00 - memory access using only register
//    01 - memory access with constant 8-bit offset
//    10 - memory access with constant 32-bit offset (we dont use it)
//    11 - no memory access, only registers
// opcode:
//    0xb8 - mem access in rm field
//    0x89 - mem access in reg field
// reg:
//    reg field -> destination/source
//    According to OperandType by module 8
// rm:
//    rm field -> source/destination
//    According to OperandType by module 8
// note: 
//    mem access with rsp and r12 requires 1 byte which is equals to 0x24. Put it after modrm and before constant offset
//    mem access with rbp and r13 without offset requires 1 byte after modrm
// 
// 
// Addition for mov <r>, <imm64>
// REX remains, and after byte which is equal to (b8 + reg num) by module 8, 
// and then 64-bit constant, where b8 is opcode
// ATTENTION R13 AND RBP IN MOV_MEM_REG IS JEBANUTY, AND RSP ESCHO BOLSHE 

/**
 * @brief returns modrm according to operands and memory access. 
 * Implies that all operands are registers
 * @param operand_a 
 * @param operand_b z
 * @param mem_access 
 * @return char 
 */
char getMODRM(OperandType operand_a, OperandType operand_b, MemoryAccessType mem_access, CommandType cmd_type = XCHG)
{
    char modrm_field = 0;

    if (mem_access == NONE) modrm_field |= (0x03 << 6);
    else if (mem_access == REG_WITH_OFFSET) modrm_field |= (0x01 << 6);
    
    modrm_field |= isRegister(operand_a) ? (operand_a % 8 << 3) : (EXTEND_COMMANDS[cmd_type] << 3);
    modrm_field |= isRegister(operand_b) ? (operand_b % 8 << 0) : (EXTEND_COMMANDS[cmd_type] << 0);

    return modrm_field;
}

char getREX(OperandType operand_a, OperandType operand_b, char rex_field)
{
    rex_field |= isNewRegister(operand_a) ? 1 << 2 : 0;
    rex_field |= isNewRegister(operand_b) ? 1 << 0 : 0;

    return rex_field;
}

char getREX(OperandType operand_a, OperandType operand_b)
{
    return getREX(operand_a, operand_b, 0x48);
}


void writeMovRegConst(Parser* parser, char* instruction, 
                      OperandType operand_a, OperandType operand_b, 
                      MemoryAccessType mem_access,
                      int64_t constant)
{           
    asmFileWrite(parser, "mov %s, %lld", REGISTERS[operand_a], constant);

    instruction[0] = getREX(operand_a, operand_b);
    instruction[1] = MOV_R_CONST + operand_a % 8;

    insertBackArray(parser->text, instruction, 2);
    insertBackArray(parser->text, (char*)&constant, sizeof(int64_t));
}

void writeMovRegMemRegOnly(Parser* parser, char* instruction,
                           MovType mov_type,
                           OperandType operand_a, OperandType operand_b, 
                           MemoryAccessType mem_access)
{
    if (atLeastOneOperandRSPorR12(mov_type, operand_a, operand_b))
    {   
        instruction[3] = RSP_MAGIC_NUMBER;
    };       
    if (mov_type == MOV_MEM_REG)
    {
        instruction[0] = getREX(operand_b, operand_a);
        instruction[2] = getMODRM(operand_b, operand_a, mem_access);
    }
    else
    {
        instruction[0] = getREX(operand_a, operand_b);
        instruction[2] = getMODRM(operand_a, operand_b, mem_access);
    }

    insertBackArray(parser->text, instruction, 3);

    if (atLeastOneOperandRBPorR13(mov_type, operand_a, operand_b)) 
    {
        pushBackArray(parser->text, instruction[3]);
    }
}

void writeMovRegMemRegOffset(Parser* parser, char* instruction, 
                          MovType mov_type,
                          OperandType operand_a, OperandType operand_b, 
                          MemoryAccessType mem_access,
                          int8_t offset)
{       
    if (mov_type == MOV_MEM_REG)
    {
        instruction[0] = getREX(operand_b, operand_a);
        instruction[2] = getMODRM(operand_b, operand_a, mem_access);
    }
    else
    {
        instruction[0] = getREX(operand_a, operand_b);
        instruction[2] = getMODRM(operand_a, operand_b, mem_access);
    }    

    if (atLeastOneOperandRSPorR12(mov_type, operand_a, operand_b)) 
    {
        instruction[3] = 0x24;
        instruction[4] = offset;
    }
    else instruction[3] = offset;

    size_t instruction_size = (atLeastOneOperandRSPorR12(mov_type, operand_a, operand_b)) ? 5 : 4;

    insertBackArray(parser->text, instruction, instruction_size);
}

void writeMovMemReg(Parser* parser, char* instruction, 
                 OperandType operand_a, OperandType operand_b, 
                 MemoryAccessType mem_access,
                 int8_t offset)
{
    instruction[1] = (char)MOV_M_RM;
    if (mem_access == REG_ONLY) 
    {   
        asmFileWrite(parser, "mov [%s], %s", 
                     REGISTERS[operand_a], REGISTERS[operand_b]);
        writeMovRegMemRegOnly(parser, instruction, MOV_MEM_REG, operand_a, operand_b, mem_access);
        return;
    }

    asmFileWrite(parser, "mov [%s + (%d)], %s", REGISTERS[operand_a], offset, REGISTERS[operand_b]);
    writeMovRegMemRegOffset(parser, instruction, MOV_MEM_REG, operand_a, operand_b, mem_access, offset);
}

void writeMovRegRegMem(Parser* parser, char* instruction, 
                 OperandType operand_a, OperandType operand_b,
                 MemoryAccessType mem_access, 
                 int8_t offset)
{   
    instruction[0] = getREX(operand_a, operand_b);
    instruction[1] = (char)MOV_R_RM;
    instruction[2] = getMODRM(operand_a, operand_b, mem_access);

    if (mem_access == NONE)
    {
        asmFileWrite(parser, "mov %s, %s", 
                     REGISTERS[operand_a], REGISTERS[operand_b]);
        insertBackArray(parser->text, instruction, 3);
        return;
    }
    if (mem_access == REG_ONLY)
    {
        asmFileWrite(parser, "mov %s, [%s]", 
                     REGISTERS[operand_a], REGISTERS[operand_b]);
        writeMovRegMemRegOnly(parser, instruction, MOV_REG_MEM, operand_a, operand_b, mem_access);
        return;
    }
    asmFileWrite(parser, "mov %s, [%s + (%d)]", REGISTERS[operand_a], REGISTERS[operand_b], offset);
    writeMovRegMemRegOffset(parser, instruction, MOV_REG_MEM, operand_a, operand_b, mem_access, offset);
}

void writePush(Parser* parser, OperandType operand)
{
    char instruction[2] = {};
    instruction[0] = getREX(VOID_OPERAND, operand, 0x40);
    instruction[1] = OPCODES[PUSH] + operand % 8;

    asmFileWrite(parser, "push %s", REGISTERS[operand]);
    insertBackArray(parser->text, instruction, 2);
}

void writePop(Parser* parser, OperandType operand)
{
    char instruction[2] = {};
    instruction[0] = getREX(VOID_OPERAND, operand, 0x40);
    instruction[1] = OPCODES[POP] + operand % 8;

    asmFileWrite(parser, "pop %s", REGISTERS[operand]);
    insertBackArray(parser->text, instruction, 2);
}

void writeSimpleTwoOperandCommand(Parser* parser, 
                                  CommandType cmd_type,
                                  OperandType operand_a, OperandType operand_b,
                                  int64_t constant = 0)
{
    char instruction[3] = {};
    instruction[0] = getREX(operand_b, operand_a);
    instruction[1] = OPCODES[cmd_type];
    instruction[2] = getMODRM(operand_b, operand_a, NONE, cmd_type);

    insertBackArray(parser->text, instruction, 3);
    if (operand_b == CONST64 || operand_b == CONST32)
    {
        asmFileWrite(parser, "%s %s, %lld", COMMANDS[cmd_type], REGISTERS[operand_a], constant);
        insertBackArray(parser->text, (char*)&constant, 
                        operand_b == CONST64 ? sizeof(int64_t) : sizeof(int32_t));
        return;
    }
    asmFileWrite(parser, "%s %s, %s", COMMANDS[cmd_type], REGISTERS[operand_a], REGISTERS[operand_b]);
}

void writeIMul(Parser* parser,
               OperandType operand_a, OperandType operand_b)
{
    char instruction[4] = {};
    asmFileWrite(parser, "imul %s, %s", REGISTERS[operand_a], REGISTERS[operand_b]);

    instruction[0] = getREX(operand_a, operand_b);
    memcpy(instruction + 1, &OPCODES[IMUL], 2);
    instruction[3] = getMODRM(operand_a, operand_b, NONE);

    insertBackArray(parser->text, instruction, 4);
}

void writeReturn(Parser* parser)
{   
    asmFileWrite(parser, "ret");
    pushBackArray(parser->text, (char)OPCODES[RET]);
}

void writeSyscall(Parser* parser)
{
    asmFileWrite(parser, "syscall");
    insertBackArray(parser->text, (char*)&OPCODES[SYSCALL], 2);
}

void writeLeave(Parser* parser)
{   
    asmFileWrite(parser, "leave");
    pushBackArray(parser->text, (char)OPCODES[LEAVE]);
}

void asmFileWrite(Parser* parser, 
                  MovType mov_type, 
                  OperandType operand_a, OperandType operand_b, 
                  MemoryAccessType mem_access,
                  int64_t constant, int8_t offset)
{   
    char instruction[16] = {};

    if (mov_type == MOV_REG_CONST64)
    {   
        writeMovRegConst(parser, instruction, 
                         operand_a, operand_b, 
                         mem_access, constant);
        return;
    }
    if (mov_type == MOV_MEM_REG)
    {   
        writeMovMemReg(parser, instruction,
                       operand_a, operand_b, 
                       mem_access, offset);
        return;
    }
    writeMovRegRegMem(parser, instruction, 
                      operand_a, operand_b, 
                      mem_access, offset);
}

void asmFileWrite(Parser* parser, 
                  CommandType cmd_type,
                  OperandType operand_a, OperandType operand_b,
                  MemoryAccessType mem_access,
                  int64_t constant, int8_t offset)
{   
    if (cmd_type >= XCHG && cmd_type <= CMP)
    {
        writeSimpleTwoOperandCommand(parser, cmd_type, operand_a, operand_b, constant);
        return;
    }
    switch (cmd_type)
    {   
        case IMUL:
            writeIMul(parser, operand_a, operand_b);
            break;

        case PUSH:
            writePush(parser, operand_a);
            break;

        case POP:
            writePop(parser, operand_a);
            break;

        case RET:
            writeReturn(parser);
            break;

        case SYSCALL:
            writeSyscall(parser);
            break;

        case LEAVE:
            writeLeave(parser);
            break;
    }
}

void asmFileWrite(Parser* parser, CommandType cmd_type, Label* label)
{
    char instruction[4] = {};
    
    asmFileWrite(parser, "%s %s", COMMANDS[cmd_type], label->name->string);

    insertBackArray(parser->text, (char*)&OPCODES[cmd_type], 
                    cmd_type >= JNE && cmd_type <= JG ? 2 : 1);
    
    insertBackArray(parser->text, instruction, 4);
    pushBackOffset(label, parser->text->size - 1);
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