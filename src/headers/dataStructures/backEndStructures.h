enum OperandType
{
    RAX,
    RCX,
    RDX,
    RBX,
    RSP,
    RBP,
    RSI,
    RDI,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,

    AMOUNT_REGS,

    MEM,
    CONST64,
    CONST32,
    VOID_OPERAND,
};

enum CommandType
{
    // MOV, <--- separate case
    XCHG, 
    XOR, 
    ADD_REGS, 
    ADD_CONST32, 
    SUB_REGS, 
    SUB_CONST32, 
    IDIV, 
    CMP, 

    IMUL, 

    RET, 
    CALL, 

    JMP, 
    JNE,
    JE,
    JLE,
    JL,
    JGE,
    JG,

    SYSCALL,
    LEAVE,

    PUSH,
    POP,

    AMOUNT_CMD
};

enum MovType
{
    MOV_REG_REG,
    MOV_REG_MEM,
    MOV_MEM_REG,
    MOV_REG_CONST64,

    AMOUNT_MOV
};

enum MemoryAccessType
{   
    NONE,
    REG_ONLY,
    REG_WITH_OFFSET,
};

enum MOV_OPCODE
{
    MOV_R_RM = 0x8b,
    MOV_M_RM = 0x89,
    MOV_R_CONST = 0xb8,
};

enum SectionOrder
{
    ZERO_HEADER,
    TEXT = 0x01,
    SHSTRTAB = 0x07,
    SYMTAB = 0x11, 
    STRTAB = 0x19,
    RELATEXT = 0x21
};

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

struct OffsetArray
{
    size_t capacity;
    size_t size;

    size_t* data;
};

struct Label
{   
    String* name;
    size_t label_offset;
    size_t strtab_offset;
    size_t symtab_idx;
    OffsetArray* offsets;
};

struct LabelTable
{
    Label* data;

    size_t capacity;
    size_t size;
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
    LabelTable* labels;
    Array* text;

    int amount_externs;

    int func_id;
    int tab_offset;
    
    int amount_if_else;
    int amount_loop;
    int amoutnt_logic;
};

struct ELF
{
    Elf64_Ehdr* elf_header;

    Elf64_Shdr* sh_text;
    Elf64_Shdr* sh_shstrtab;
    Elf64_Shdr* sh_symtab;
    Elf64_Shdr* sh_strtab;
    Elf64_Shdr* sh_relatext;

    Array* text;
    Array* shstrtab;
    Array* symtab;
    Array* strtab;
    Array* relatext;

    size_t main_func_strtab_offset;
};

