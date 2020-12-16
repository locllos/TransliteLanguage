enum TokenType
{
    MATH_EQUAL,
    MATH_NOT_EQUAL,
    MATH_EQUAL_LESS,
    MATH_EQUAL_GREATER,
    MATH_LESS,
    MATH_GREATER,
    
    MATH_PLUS,
    MATH_MINUS,
    MATH_MUL,
    MATH_DIV,

    COMMA,
    SEMICOLON,
    
    OPEN_BRACE,
    CLOSE_BRACE,

    OPEN_BRACKET,
    CLOSE_BRACKET,

    DECLARATOR,
    ASSIGNMENT,

    COND_IF,
    COND_ELSE,

    LOOP_WHILE,

    RETURN,

    // PRINT,
    // SCAN,

    AMOUNT_TOKENS,

    NUMBER,
    NAMING,
    
    VOID,
};

enum Type
{
    DEFINITION_TYPE,
    DECLARATION_TYPE,
    STATEMENT_TYPE,
    COMPOUND_TYPE,
    IF_ELSE_TYPE,
    ASSIGN_TYPE,
    CALL_TYPE,
    MATH_TYPE,
    LOOP_TYPE,
    JUMP_TYPE,
    ARG_TYPE,
    IF_TYPE,

    NAMING_TYPE,
    NUMBER_TYPE,

    ERROR_TYPE,
};

struct String
{
    char* string;
    size_t length;

    size_t capacity;
};

union Value
{
    String naming;
    double number;
};
 
struct Token
{
    Value value;
    TokenType type;
    size_t line;
};

struct TokenArray
{
    Token* array;

    size_t size;
    size_t capacity;
};

struct Node
{
    Node* left;
    Node* right;
    // Node* parent;

    Type type;
    Value value;
};

struct Tree
{
    Node* root;
};

struct ListNode
{
    ListNode* next;

    Token* value;
};

struct List
{
    ListNode* root;
    size_t size;
};