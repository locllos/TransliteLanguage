#ifndef CONSTANTS
#define CONSTANTS
static const size_t BASE_VAR_ARRAY_SIZE = 8;
static const size_t BASE_FUNC_ARRAY_SIZE = 3;
static const char MAIN[] = "zaglaveeye";
static const char PRINT[] = "raspechatayty";
static const char INPUT[] = "vvod";
static const char SQRT[] = "koreyn";
static const size_t NOT_INIT = 0xffffffff;
static const size_t MAX_LABEL_NAME_SIZE = 64;
static const size_t START_OFFSET_ARRAY_CAPACITY = 4;
static const size_t DEFAULT_START_CAPACITY = 16;
static const size_t gap_size = 8;

static const String TOKEN[]
{    
    {"==", 2, 2},
    
    {"!=", 2, 2},

    {"<=", 2, 2},

    {">=", 2, 2},
    
    {"<", 1, 1},
    {">", 1, 1},
    {"+", 1, 1},
    {"-", 1, 1},
    {"*", 1, 1},
    {"/", 1, 1},

    {",", 1, 1},
    {"tchk", 4, 4},
    
    // {"{", 1, 1},
    {"__/", 3, 3},
    
    // {"}", 1, 1},
    {"|", 1, 1},

    {"(", 1, 1},
    {")", 1, 1},

    // {"declalator", 10, 10},
    {"obbyaveety", 10, 10},
    // {"=", 1, 1},
    {"preesvoytee", 11, 11},

    // {"if", 2, 2},
    {"yeslee", 6, 6},
    // {"else", 4, 4},
    {"eenachee", 8, 8},

    // {"while", 5, 5},
    {"pokoodah", 8, 8},
    
    // {"return", 6, 6},
    {"verneety", 8, 8},

    // {"print", 5, 5},
    // {"scan", 4, 4},

};


static const char* NODE_TYPE[]
{
    "def",
    "decl",
    "stat",
    "comp",
    "if/else",
    "assign",
    "call",
    "math",
    "while",
    "ret",
    "arg",
    "if"
};

static const char* REGISTERS[]
{
    "rax",
    "rcx",
    "rdx",
    "rbx",
    "rsp",
    "rbp",
    "rsi",
    "rdi",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",
};

static const char* COMMANDS[]
{

    "xchg",
    "xor",
    "add",
    "add",
    "sub",
    "sub",
    "idiv",
    "cmp",

    "imul",

    "ret",
    "call",


    "jmp",
    "jne",
    "je",
    "jle",
    "jl",
    "jge",
    "jg",

    "syscall"
    "leave",

    "push",
    "pop",
};

static const size_t OPCODES[]
{
    0x87,   // xchg
    0x31,   // xor
    0x01,   // add_regs
    0x81,   // add_const
    0x29,   // sub_regs
    0x81,   // sub_const
    0xf7,   // idiv
    0x39,   // cmp
    
    0xaf0f, // imul

    0xc3,   // ret
    0xe8,   // call

    0xe9,     // jmp
    0x850f,   // jne
    0x840f,   // je
    0x8e0f,   // jle
    0x8c0f,   // jl
    0x8d0f,   // jge
    0x8f0f,   // jg

    0x050f,   // syscall
    0xc9,     // leave

    0x50,     // push
    0x58,     // pop
};

static int EXTEND_COMMANDS[]
{   
    0x00,
    0x00,
    0x00,
    0x00, //add const
    0x00, 
    0x05, //sub const
    0x07, //idiv
    0x00,
};
#endif