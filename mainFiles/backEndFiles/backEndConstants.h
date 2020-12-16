const size_t BASE_VAR_ARRAY_SIZE = 8;
const size_t BASE_FUNC_ARRAY_SIZE = 3;
FILE* compilator_logs = fopen("serviceFiles/comp_logs.txt", "w");
const char* MAIN = "zaglaveeye";
const char* PRINT = "raspechatayty";
const char* INPUT = "vvod";
const char* SQRT = "koreyn";

const String TOKEN[]
{    
    {"==", 2, 2},
    // {"ravno", 5, 5},
    
    {"!=", 2, 2},
    // {"ne ravno"}

    {"<=", 2, 2},
    // {"men'shee_eelee_ravno", 20, 20},

    {">=", 2, 2},
    // {"bol'she_eelee_ravno", 19, 19},
    
    {"<", 1, 1},
    {">", 1, 1},
    {"+", 1, 1},
    {"-", 1, 1},
    {"*", 1, 1},
    {"/", 1, 1},

    {",", 1, 1},
    // {";", 1, 1},
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


const char* NODE_TYPE[]
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