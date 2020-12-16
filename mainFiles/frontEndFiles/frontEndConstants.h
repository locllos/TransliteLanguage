const String TOKENS[]
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


const char* NODE_TYPES[]
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

const char* MATHS[]
{
    "\\==",
    "\\<=",
    "\\>=",
    "\\<", 
    "\\>", 
    "\\+", 
    "\\-", 
    "\\*", 
    "\\/", 

};

const char DUMP_TYPE_TRANSALATOR[] = 
{
    0,
    1,
    5,
    4,
    7,
    9,
    10,
    12,
    8,
    11,
    3,
    6,
    2,
    13
};

const char DUMP_MATH_TRANSLATOR[] = 
{
    4,
    5,
    6,
    7,

    0,
    1,
    2,
    3,
};

const char TYPE_TRANSALATOR[] = 
{
    0,
    1,
    12,
    10,
    3,
    2,
    11,
    4,
    8,
    10,
    6,
    9,
    7,
    13    
};

const char MATH_TRANSLATOR[] = 
{
    6, 
    7,
    8,
    9,
    0,
    1,
    2,
    3,
    4,
    5
};

const size_t TOKENS_BASE_CAPACITY = 512;
const size_t STRING_BASE_CAPACITY = 8;
List* ERROR_LIST = nullptr;
const char* DOT_COMMAND = "dot -Tpng ";
FILE* logs = fopen("serviceFiles/logs.txt", "w");