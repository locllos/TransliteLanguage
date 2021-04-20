#ifndef CONSTANTS
#define CONSTANTS
static const size_t BASE_VAR_ARRAY_SIZE = 8;
static const size_t BASE_FUNC_ARRAY_SIZE = 3;
static const char MAIN[] = "zaglaveeye";
static const char PRINT[] = "raspechatayty";
static const char INPUT[] = "vvod";
static const char SQRT[] = "koreyn";

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
#endif