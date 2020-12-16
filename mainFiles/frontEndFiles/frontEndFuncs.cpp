#include "../headerFiles/frontEndHeader.h"
#include "frontEndConstants.h"

#define dbg
#define dmp

#ifndef dbg
#define dbg printf("(func: %s | line: %d)\n", __func__, __LINE__)
#endif

#ifndef dmp
#define dmp fprintf(logs, "(func: %s | line: %d)\n", __func__, __LINE__); showToken(&tokens->array[*pos], *pos) 
#endif

//Service funcs
void systemCall(const char* filename, const char* graph_filename)
{
    char command[96] = {};

    size_t i = 0;
    while (DOT_COMMAND[i] != '\0')
    {
        command[i] = DOT_COMMAND[i];
        ++i;
    }
    size_t j = 0;
    while (filename[j] != '\0')
    {
        command[i] = filename[j++];
        ++i;
    }
    command[i++] = ' ';
    command[i++] = '-';
    command[i++] = 'o';
    command[i++] = ' ';
    j = 0;
    while (graph_filename[j] != '\0')
    {
        command[i] = graph_filename[j++];
        ++i;
    }
    
    system(command);
}

int approxLength(const char* filename)
{
	assert(filename);
	
	struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

	stat(filename, buff);

	return buff->st_size;
}

char* readFile(const char* filename, size_t* buffer_size)
{
    FILE* file = fopen(filename, "r");

    assert(file);
    
    size_t apr_size = approxLength(filename);

    char* buffer = (char*)calloc(apr_size, sizeof(char));
    
    apr_size =  fread(buffer, sizeof(char), apr_size, file);

    if (buffer_size != nullptr) *buffer_size = apr_size;

    if (buffer != nullptr) buffer[apr_size] = '\0';

    return buffer;
}

void skipUselessSymbols(char* string, size_t* pos, size_t* line)
{   
    size_t i = *pos;
    while (isspace(string[i]))
    {   
        if (string[i] == '\n') ++*line;
        ++i;
    }
    *pos = i;
}

void printStr(const char* string)
{
    printf("|%s|\n", string);
}

bool isEqualFirstSymbols(const char* string_a, const char* string_b)
{   
    size_t j = 0;
    bool is_equal = false;
    // printf("%c <> %c\n", string_a[j], string_b[j]);
    while (string_a[j] != '\0' && string_b[j] != '\0' &&
            (is_equal = string_a[j] == string_b[j]) == true)
    {   
        // printf("%c <> %c\n", string_a[j], string_b[j]);
        ++j;
    }
    return is_equal;
}

int getTokenType(char* string, size_t* pos)
{   
    dbg;
    bool is_equal = false;
    for (size_t i = 0; i < AMOUNT_TOKENS; ++i)
    {    
        // printf("Cur: |%s|\n", TOKENS[i].string);
        if (isEqualFirstSymbols(TOKENS[i].string, string + *pos)) 
        {   
            // printf("find: (%s|%zu|%zu)\n", TOKENS[i].string, TOKENS[i].length, TOKENS[i].capacity);
            *pos += TOKENS[i].length;
            dbg;
            return i;
        }
    }
    dbg;
    return -1;
}

inline bool isValidSymbol(char symbol)
{
    return ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z') || symbol == '_');
}

bool Require(const String* string, const char* buffer, size_t* pos)
{
    if (isEqualFirstSymbols(string->string, buffer + *pos))
    {
        *pos += string->length;
        return true;
    }
    return false;
}

bool Require(const String* string_a, const String* string_b, size_t* pos)
{
    if (strcmp(string_a->string, string_b->string) == 0)
    {
        *pos += string_b->length;
        return true;
    }
    return false;
}

bool Require(TokenArray* tokens, size_t* pos, TokenType type)
{
    if (*pos < tokens->size && getTokenType(tokens, pos)  == type)
    {   
        ++*pos;
        return true;
    }
    return false;
}

inline TokenType getTokenType(TokenArray* tokens, size_t* pos)
{
    return tokens->array[*pos].type;
}

inline TokenType getNextTokenType(TokenArray* tokens, size_t* pos)
{
    return (tokens->size <= *pos + 1) ? VOID : tokens->array[*pos + 1].type;
}

inline bool isLogicOperator(TokenType type)
{
    return (type >= MATH_EQUAL && type <= MATH_GREATER);
}

inline bool isSimpleOperator(TokenType type)
{
    return (type == MATH_PLUS || type == MATH_MINUS);
}

inline bool isTermOperator(TokenType type)
{
    return (type == MATH_MUL || type == MATH_DIV);
}

void showErrors(List* error_list)
{
    ListNode* cur_node = error_list->root;
    while (cur_node != nullptr)
    {
        printf("%zu: ", cur_node->value->line);
        showToken(cur_node->value);
        cur_node = cur_node->next;
    }
}

//List
ListNode* newListNode(ListNode* next, Token* value)
{
    ListNode* new_node = (ListNode*)calloc(1, sizeof(ListNode));

    new_node->value = value;
    new_node->next = next;

    return new_node;
}

void constructList(List* list)
{
    list->root = nullptr;
    list->size = 0;
}

List* newList()
{
    List* new_list = (List*)calloc(1, sizeof(List));

    constructList(new_list);

    return new_list;
}

void pushList(List* list, Token* value)
{
    if (list->root == nullptr)
    {
        list->root = newListNode(nullptr, value);
        ++list->size;
        return;
    }
    ListNode* cur_node = list->root;
    list->root = newListNode(cur_node, value);
    ++list->size;
    // showErrors(list);
}

void popList(List* list)
{
	if (list->size < 1)
	{
		return;
	}
	
	ListNode* node = list->root;
	list->root = node->next;

	--list->size;
	free(node);
}

void destructList(List* list)
{   
    while (list->size > 0)
    {  
        popList(list);
    }
}

//Node
void constructNode(Node* node)
{
    node->left = nullptr;
    node->right = nullptr;
    node->type = ERROR_TYPE;
    node->value = {};
}

Node* newNode()
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    constructNode(new_node);

    return new_node;
}

Node* newNode(Type type, Value value)
{
    Node* new_node = newNode();
    new_node->type = type;
    new_node->value = value;

    return new_node;
}

Node* newNode(Node* left_node, Node* right_node, Type type, Value value)
{
    Node* new_node = newNode(type, value);

    new_node->left = left_node;
    new_node->right = right_node;

    return new_node;
}

void relinkNode(Node* relinkable_node, Node* left, Node* right)
{
    relinkable_node->left = left;
    relinkable_node->right = right;
}

Type getNodeType(Node* node)
{
    return (node->type);
}

//Value
/*
Value createValue(const ValueType value_type, ...)
{   
    dbg;
    Value value = {};

    va_list begin;
    va_start(begin, value_type);


    switch (value_type)
    {
        case NUM:
            value.number = va_arg(begin, double);
            printf("Create number: (%lg)\n", value.number);
            break;
        
        case NAM:
            value.naming = va_arg(begin, String);
            printf("Create string structure: (%s|%zu|%zu)\n", 
                    value.naming.string, value.naming.length, value.naming.capacity);
            break;
        
        default:
            printf("(empty)\n");
            value.number = -1;
    }
    
    va_end(begin);
    dbg;
    return value;
}
*/

Value createValue(String string)
{
    dbg;
    Value value = {};
    value.naming = string;
    dbg;
    return value;
}

Value createValue(double number)
{
    dbg;
    Value value = {};
    value.number = number;
    dbg;
    return value;
}

Value createValue()
{
    dbg;
    Value value = {};
    value.naming = {nullptr, 0, 0};
    dbg;
    return value;
}

//String
void constructString(String* string)
{
    string->string = (char*)calloc(STRING_BASE_CAPACITY + 1, sizeof(char));
    string->capacity = STRING_BASE_CAPACITY;
    string->length = 0;
    string->string[0] = '\0';
}

String* newString()
{
    String* string = (String*)calloc(1, sizeof(String));

    constructString(string);

    return string;
}

void reallocateString(String* string)
{
    string->capacity += 10;
    string->string = (char*)realloc(string->string, string->capacity * sizeof(char) + 1);
}

void stringAppend(String* string, char symbol)
{
    if (string->length + 1 > string->capacity)
    {
        reallocateString(string);
    }
    string->string[string->length++] = symbol;
    string->string[string->length] = '\0';
}

void destructString(String* string)
{
    free(string->string);
    string->capacity = 0;
    string->length = 0;
}

//TokenArray
void constructTokenArray(TokenArray* tokens, size_t base_capacity)
{
    tokens->array = (Token*)calloc(base_capacity, sizeof(Token));
    for (size_t i = 0; i < base_capacity; ++i)
    {
        tokens->array[i].type = VOID;
    }
    tokens->size = 0;
    tokens->capacity = base_capacity;
}

TokenArray* newTokenArray(size_t base_capaicity)
{
    TokenArray* new_tokens = (TokenArray*)calloc(1, sizeof(TokenArray));

    constructTokenArray(new_tokens, base_capaicity);

    return new_tokens;
}

void reallocateTokenArray(TokenArray* tokens)
{
    tokens->capacity = 2 * tokens->capacity;
    tokens->array = (Token*)realloc(tokens->array, tokens->capacity * sizeof(Token));
}

void tokenArrayAppend(TokenArray* tokens, TokenType type, size_t line, Value value)
{   
    dbg;
    if (tokens->size + 1 > tokens->capacity)
    {
        reallocateTokenArray(tokens);
    }
    tokens->array[tokens->size].type = type;
    tokens->array[tokens->size].line = line;
    tokens->array[tokens->size].value = value;
    ++tokens->size;
    dbg;
}

void destructTokenArray(TokenArray* tokens)
{
    free(tokens->array);
    tokens->capacity = 0;
    tokens->size = 0;
}

Token* getToken(TokenArray* tokens, size_t* pos)
{
    return &tokens->array[*pos];
}

inline String* getTokenString(Token* token)
{
    return  &token->value.naming;
}

inline double getTokenNumber(Token* token)
{
    return token->value.number;
}

//Tokenization
String getNaming(char* buffer, size_t* pos)
{
    dbg;
    String string = {nullptr, 0, 0};

    size_t i = *pos;
    while (isValidSymbol(buffer[i]) || (isdigit(buffer[i]) && i > *pos))
    {   
        // printf("cur: %c\n", buffer[i]);
        if (string.capacity == 0)
        {
            constructString(&string);
        }
        stringAppend(&string, buffer[i++]);
        
    }
    *pos = i;
    dbg;
    return string;
}

double getNumber(char* string, size_t* pos)
{   
    dbg;
    char* begin_number = string + *pos;
    char* end_number = nullptr;

    double value = 0;
    value = strtod(begin_number, &end_number);
    if (begin_number == end_number)
    {
        dbg;
        return NAN;
    }
    *pos += end_number - begin_number;
    dbg;
    return value;
}

void showToken(Token* token, int i)
{   
    if (i > -1)
    {
        printf("Token[%d]: ", i);
    }
    if (token->type == NUMBER)
    {
        printf("[number |Value: %lg | Line: %zu]\n", 
            token->value.number, token->line);
    }
    else if (token->type == NAMING)
    {
        printf("[naming | Name: %s | Line: %zu]\n", 
            token->value.naming.string, token->line);
    }
    else if (token->type >= MATH_EQUAL &&
                token->type <= RETURN)
    {
        printf("[KeyWord | Key: %s | Line: %zu]\n", 
            TOKENS[token->type].string, token->line);
    }
    else printf("(empty)\n");
}

void showTokens(TokenArray* tokens)
{
    for (size_t i = 0; i < tokens->size; ++i)
    {           
        showToken(&tokens->array[i], (int)i);
    }
}

TokenArray* getTokens(char* buffer)
{   
    dbg;
    TokenArray* tokens = newTokenArray(TOKENS_BASE_CAPACITY);
    size_t line = 1;
    String naming = {};
    double number = 0;
    int idx = 0;
    size_t pos = 0;

    constructTokenArray(tokens, TOKENS_BASE_CAPACITY);

    while (buffer[pos] != '\0')
    {   
        skipUselessSymbols(buffer, &pos, &line);
        // printStr(buffer + pos);
        if (!isnan(number = getNumber(buffer, &pos)))
        {   
            // printf("Find number: %lg\n", number);
            tokenArrayAppend(tokens, NUMBER, line, createValue(number));
        }
        else if ((idx = getTokenType(buffer, &pos)) > -1)
        {   
            // printf("Find operation: %s\n", TOKENS[idx].string);
            tokenArrayAppend(tokens, (TokenType)idx, line, createValue());
        }
        else if ((naming = getNaming(buffer, &pos)).length > 0)
        {   
            // printf("Find naming: %s\n", naming.string);
            tokenArrayAppend(tokens, NAMING, line, createValue(naming));
        }
        else
        {   
            // printf("Find nothing. Pass...\n");
            ++pos;
        }
        dbg;
        // printf("---\n");
    }
    return tokens;
}


//Recursive descent
#define SYNTAX_ERROR(node)    \
    pushList(ERROR_LIST, &tokens->array[(*pos - 1 >= 0) ? *pos - 1 : 0]); \
    return node;

#define SYNTAX_ASSERT(node)     \
    if (node == nullptr) { printf("[%d]error token ->", __LINE__); showToken(getToken(tokens, pos)); SYNTAX_ERROR(nullptr); }                           

Node* getGeneral(TokenArray* tokens)
{   
    dbg;
    ERROR_LIST = newList();

    size_t pos = 0;
    Node* result_node = getProgramm(tokens, &pos);
    if (pos != tokens->size)
    {
        printf("END OF FILE WAS NOT REACHED\n");
    }
    if (ERROR_LIST->size > 0)
    {   
        printf("ERRORR!!!\n");
        printf("amount erros: %zu\n", ERROR_LIST->size);
        showErrors(ERROR_LIST);
    }
    dbg;
    return result_node;
}

Node* getProgramm(TokenArray* tokens, size_t* pos)
{
    dmp;
    Node* definition_node = getDefinition(tokens, pos);
    Node* head_node = definition_node;
    Node* linker_node = definition_node;

    while (definition_node != nullptr)
    {   
        definition_node = getDefinition(tokens, pos);
        linker_node->left = definition_node;
        linker_node = definition_node;
    }
    return head_node;
}

Node* getDefinition(TokenArray* tokens, size_t* pos)
{
    dmp;
    Node* declaration_node = getFunctionDeclaration(tokens, pos);

    if (declaration_node == nullptr) return nullptr;

    Node* block_node = getBlock(tokens, pos);

    SYNTAX_ASSERT(block_node);

    declaration_node->left = block_node;
    dmp;
    fprintf(logs, "---Definition---\n");
    showNode(declaration_node);
    return newNode(nullptr, declaration_node, DEFINITION_TYPE, {});
}

Node* getFunctionDeclaration(TokenArray* tokens, size_t* pos)
{   
    dmp;
    if (!Require(tokens, pos, DECLARATOR)) return nullptr;

    Node* naming = getNaming(tokens, pos);

    SYNTAX_ASSERT(naming);

    Node* parametrs = getParametrList(tokens, pos);

    naming->right = parametrs;
    fprintf(logs, "---Function Declaration---\n");
    showNode(naming);
    dmp;
    return naming; 
}

Node* getParametrList(TokenArray* tokens, size_t* pos)
{   
    dmp;
    Node* linker_node = nullptr;
    Node* parametr_node = nullptr;
    Node* first_parametr_node = nullptr;
    if (!Require(tokens, pos, OPEN_BRACKET)) { SYNTAX_ERROR(nullptr); }
    
    while ((parametr_node = getNaming(tokens, pos)) != nullptr)
    {   
        if (linker_node != nullptr) linker_node->right = parametr_node;
        else first_parametr_node = parametr_node;
                
        linker_node = parametr_node;
        
        if (!Require(tokens, pos, COMMA)) break;
        dmp;
    }
    if (!Require(tokens, pos, CLOSE_BRACKET)) { SYNTAX_ERROR(first_parametr_node); }

    dmp;
    fprintf(logs, "---ParamList---\n");
    showNode(first_parametr_node);
    return first_parametr_node;
}

Node* getArgumentList(TokenArray* tokens, size_t* pos)
{   
    dmp;
    Node* linker_node = nullptr;
    Node* argument_node = nullptr;
    Node* first_argument_node = 0;
    Node* expression_node = nullptr;
    if (!Require(tokens, pos, OPEN_BRACKET)) { SYNTAX_ERROR(nullptr); }
    
    while ((expression_node = getExpression(tokens, pos)) != nullptr)
    {   
        argument_node = newNode(expression_node, linker_node, ARG_TYPE, {});

        linker_node = argument_node;
        
        if (!Require(tokens, pos, COMMA)) break;
        dmp;
    }
    // if (expression_node == nullptr) ++*pos;

    if (!Require(tokens, pos, CLOSE_BRACKET)) { SYNTAX_ERROR(nullptr); }

    dmp;
    fprintf(logs, "---ArgumentList---\n");
    showNode(argument_node);
    return argument_node;
}

Node* getBlock(TokenArray* tokens, size_t* pos)
{    
    dmp;
    fprintf(logs, "\\__Begin block\n");
    Node* linker_node = nullptr;
    Node* statement_node = nullptr;  
    Node* first_statement_node = nullptr;

    if (!Require(tokens, pos, OPEN_BRACE)) return nullptr;  

    while ((statement_node = getStatement(tokens, pos)) != nullptr)                                                                                    
    {                       
        if (linker_node != nullptr) linker_node->right   = statement_node;
        else                        first_statement_node = statement_node;
        
        linker_node = statement_node;
        dmp;
    }
    
    if (!Require(tokens, pos, CLOSE_BRACE)) { SYNTAX_ERROR(nullptr); }
    Node* block_node = newNode(first_statement_node, nullptr, COMPOUND_TYPE, {});

    dmp;
    fprintf(logs, "---Block---\n");
    showNode(block_node);
    fprintf(logs, "\\__End block\n");
    return block_node;
}

Node* getStatement(TokenArray* tokens, size_t* pos)
{
    /*
    Да. Да, я упоролся. Ну да, я это признаю. Что ты говоришь? 
    Почему у меня столько элайнов? Ну, просто мне нравятся эти элайны.
    Поддерживаю ли я ЛГБТ? Да.
    Да, я являюсь частью сообщества. А почему ты спрашиваешь?
    В смысле навязываю тебе что-то? Так ты же сам спросил. Ладно.
    Хочу ли я, чтобы мой друг делал элайны? Боже, нет, конечно. Почему я должен этого хотеть?
    В смысле у всех? Нет, постой, это не так работает немножко. Тебе объяснить?
    Не надо пропагандировать? Я не пропагандирую, ты просто сам спросил у меня… 
    Ясно, я сумасшедший. Как и все. Ладно, извини, что потревожил. Я отсяду.
    */
    dmp;
    fprintf(logs, "\\__Begin statemente\n");
    Node* statement_node = nullptr;

    if      ((statement_node  = getCondition (tokens, pos))  != nullptr) {fprintf(logs, "---Condition---\n");}
    else if ((statement_node  = getLoop      (tokens, pos))  != nullptr) {fprintf(logs, "---Loop---\n");}
    else if (((statement_node = getReturn    (tokens, pos))  != nullptr) ||
            ((statement_node  = getAssign    (tokens, pos))  != nullptr) ||
            ((statement_node  = getExpression(tokens, pos))  != nullptr))
    {
        if (!Require(tokens, pos, SEMICOLON))
        {
            pushList(ERROR_LIST, &tokens->array[*pos]);
        }
        fprintf(logs, "---Ret or  Assign or Expr---\n");
    }
    else return nullptr;

    showNode(statement_node);
    Node* linker_node = newNode(statement_node, nullptr, STATEMENT_TYPE, {});
    dmp;
    fprintf(logs, "\\__End statemente\n");
    return linker_node;
}

Node* getCondition(TokenArray* tokens, size_t* pos)
{
    dmp;
    if (!Require(tokens, pos, COND_IF)) return nullptr;

    if (!Require(tokens, pos, OPEN_BRACKET)) { SYNTAX_ERROR(nullptr); }

    Node* expression_node = getExpression(tokens, pos);

    SYNTAX_ASSERT(expression_node);

    if (!Require(tokens, pos, CLOSE_BRACKET)) { SYNTAX_ERROR(expression_node); }

    Node* block_node_true = getBlock(tokens, pos);

    SYNTAX_ASSERT(block_node_true);

    Node* block_node_false = nullptr;
    if (Require(tokens, pos, COND_ELSE))
    {
        block_node_false = getBlock(tokens, pos);
    }

    SYNTAX_ASSERT(block_node_true);
    dmp;
    return newNode(expression_node, 
            newNode(block_node_true, block_node_false, IF_ELSE_TYPE, {}), IF_TYPE, {});
}

Node* getLoop(TokenArray* tokens, size_t* pos)
{
    dmp;
    if (!Require(tokens, pos, LOOP_WHILE)) return nullptr;

    if (!Require(tokens, pos, OPEN_BRACKET)) { SYNTAX_ERROR(nullptr); }

    Node* expression_node = getExpression(tokens, pos);
    
    SYNTAX_ASSERT(expression_node);

    if (!Require(tokens, pos, CLOSE_BRACKET)) { SYNTAX_ERROR(nullptr); }

    Node* block_node = getBlock(tokens, pos);
    
    SYNTAX_ASSERT(block_node);
    dmp;
    return newNode(expression_node, block_node, LOOP_TYPE, {});
}

Node* getCall(TokenArray* tokens, size_t* pos)
{   
    dmp;
    if (getNextTokenType(tokens, pos) != OPEN_BRACKET) return nullptr;
    //Перепрыгивать не надо, это сделает getParamList

    Node* naming_node = getNaming(tokens, pos);
    
    SYNTAX_ASSERT(naming_node);
    Node* argument_node = getArgumentList(tokens, pos);
    
    dmp;
    Node* call_node = newNode(naming_node, argument_node, CALL_TYPE, {});
    return call_node;
}


Node* getAssign(TokenArray* tokens, size_t* pos)
{
    dmp;
    if (getNextTokenType(tokens, pos) != ASSIGNMENT) return nullptr;

    Node* naming_node = getNaming(tokens, pos);

    ++*pos; //jump over '=', We know that here assignment;

    SYNTAX_ASSERT(naming_node);

    Node* expression_node = getExpression(tokens, pos);

    SYNTAX_ASSERT(expression_node);
    dmp;
    return newNode(naming_node, expression_node, ASSIGN_TYPE, {});
}

Node* getReturn(TokenArray* tokens, size_t* pos)
{
    dmp;
    if (!Require(tokens, pos, RETURN)) return nullptr;

    Node* expression_node = getExpression(tokens, pos);

    SYNTAX_ASSERT(expression_node);
    dmp;
    return newNode(nullptr, expression_node, JUMP_TYPE, {});
}

Node* getExpression(TokenArray* tokens, size_t* pos)
{
    dmp;
    Node* simple_node_a = getSimple(tokens, pos);

    if (!isLogicOperator(getTokenType(tokens, pos))) return simple_node_a;
    //Позволяю только один логический оператор

    double token_type = getTokenType(tokens, pos);
    ++*pos;

    Node* simple_node_b =  getSimple(tokens, pos);

    SYNTAX_ASSERT(simple_node_b);
    dmp;
    return newNode(simple_node_a, simple_node_b, MATH_TYPE, createValue(token_type));
}

Node* getSimple(TokenArray* tokens, size_t* pos)
{
    dmp;
    Node* relinkable_term_node = getTerm(tokens, pos);

    fprintf(logs, "---Simple---\n");
    showNode(relinkable_term_node);
    Node* term_node = nullptr;
    double token_type = 0;

    while (isSimpleOperator(getTokenType(tokens, pos)))
    {
        token_type = getTokenType(tokens, pos);
        ++*pos;

        term_node = getTerm(tokens, pos);

        SYNTAX_ASSERT(term_node);

        relinkable_term_node = newNode(relinkable_term_node, term_node, 
                                        MATH_TYPE, createValue(token_type));
        fprintf(logs, "---Simple---\n");
        showNode(relinkable_term_node);
        dmp;
    }
    return relinkable_term_node;
}

Node* getTerm(TokenArray* tokens, size_t* pos)
{
    dmp;
    Node* relinkable_prim_node = getPrimaryExpression(tokens, pos);
    fprintf(logs, "---Term---\n");
    showNode(relinkable_prim_node);
    Node* prim_node = nullptr;
    double token_type = 0;

    while (isTermOperator(getTokenType(tokens, pos)))
    {
        token_type = getTokenType(tokens, pos);
        ++*pos;

        prim_node = getPrimaryExpression(tokens, pos);

        SYNTAX_ASSERT(prim_node);

        relinkable_prim_node = newNode(relinkable_prim_node, prim_node, 
                                        MATH_TYPE, createValue(token_type));
        fprintf(logs, "---Term---\n");
        showNode(relinkable_prim_node);
        dmp;
    }
    return relinkable_prim_node;
}

Node* getPrimaryExpression(TokenArray* tokens, size_t* pos)
{
    dmp;
    if (!Require(tokens, pos, OPEN_BRACKET)) return getBottom(tokens, pos);

    Node* expression_node = getExpression(tokens, pos);

    if (!Require(tokens, pos, CLOSE_BRACKET)) { SYNTAX_ERROR(expression_node); }
    dmp;
    fprintf(logs, "---PrimExpr\n");
    showNode(expression_node);
    return expression_node;
}

Node* getBottom(TokenArray* tokens, size_t* pos)
{
    dmp;
    Node* bottom_node = nullptr;
    if      ((bottom_node = getCall(tokens, pos))   != nullptr) {fprintf(logs, "---CALL---\n");}
    else if ((bottom_node = getNaming(tokens, pos)) != nullptr) {fprintf(logs, "---NAMING---\n");}
    else if ((bottom_node = getNumber(tokens, pos)) != nullptr) {fprintf(logs, "---NUMBER---\n");}
    else return nullptr;
    fprintf(logs, "---Bottom---");
    showNode(bottom_node);

    dmp;
    return bottom_node;
}

Node* getNaming(TokenArray* tokens, size_t* pos)
{
    dmp;
    Value naming = getToken(tokens, pos)->value;
    if (!Require(tokens, pos, NAMING)) return nullptr;

    dmp;
    return newNode(nullptr, nullptr, NAMING_TYPE, naming);
}

Node* getNumber(TokenArray* tokens, size_t* pos)
{
    dmp;
    Value number = getToken(tokens, pos)->value;
    if (!Require(tokens, pos, NUMBER)) return nullptr;

    dmp;
    return newNode(nullptr, nullptr, NUMBER_TYPE, number);
}

#undef SYNTAX_ASSERT
#undef SYNTAX_ERROR
#undef dmp
#undef dbg

//Node
void printNode(Node* node)
{
    switch (node->type)
    {
        case NAMING_TYPE:
            fprintf(logs, "(NAMING: [%s])", node->value.naming.string); 
            break;

        case NUMBER_TYPE:
            fprintf(logs, "(NUMBER: [%lg])", node->value.number); 
            break;

        case MATH_TYPE:
            fprintf(logs, "(MATH: [%s])\n", TOKENS[(int)node->value.number].string); 
            break;
        
        default:
            if (node->type >= DEFINITION_TYPE && node->type <= IF_TYPE)
            {
                fprintf(logs, "(KEY: [%s])", NODE_TYPES[node->type]);
            }
            else
            {
                fprintf(logs, "{void}");
            }
            break;
    }
    fprintf(logs, "\n");
}

void showNode(Node* node)
{
    if (node == nullptr)
    {
        fprintf (logs, "[\n(nullptr)\n]\n");
        return;
    }
    fprintf(logs, "[\n");

    printNode(node);
    
    fprintf(logs, "left: ");
    if (node->left != nullptr)
    {
        printNode(node->left);
    }
    else
    {
        fprintf(logs, "nullptr\n");
    }
    fprintf(logs, "right: ");
    if (node->right != nullptr)
    {
        printNode(node->right);    
    }
    else
    {
        fprintf(logs, "nullptr\n");
    }
    
    fprintf(logs, "]\n");
}

void printNodeValue(FILE* file, Node* node)
{
    switch (node->type)
    {
        case NAMING_TYPE:
            fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"seagreen1\", label=\"{%s}\"]\n ", 
                    node, node->value.naming.string); 
            break;

        case NUMBER_TYPE:
            fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"lightpink\", label=\"{%lg}\"]\n ", 
                    node, node->value.number); 
            break;

        case MATH_TYPE:
            fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"blue\", label=\"{%s}\"]\n ", 
                    node, MATHS[(int)node->value.number]); 
            break;
        
        default:
            if (node->type >= DEFINITION_TYPE && node->type <= IF_TYPE)
            {
                fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"skyblue1\", label=\"{%s}\"]\n ", 
                    node, NODE_TYPES[node->type]);
            }
            else
            {
                fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"seashell1\", label=\"{void}\"]\n ", 
                        node);
            }
            break;
    }
}

void loadNodes(FILE* file, Node* node)
{
    if (node == nullptr)
    {
        return;
    }
    if (node->left != nullptr)
    {
        fprintf(file, "\"%p\":sw->\"%p\"\n", node, node->left);  // link parent and left child
        loadNodes(file, node->left);
    }
    if (node->right != nullptr)
    {
        fprintf(file, "\"%p\":se->\"%p\"\n", node, node->right);  // link parent and right child
        loadNodes(file, node->right);
    }
    showNode(node);
    printNodeValue(file, node);
}

void drawGraph(Node* node, const char* filename)
{   
    fprintf(logs, "drawing graph...\n");
    if (node == nullptr) 
    {
        return;
    }
    FILE* file = fopen("serviceFiles/graphPlot.txt", "w");

    fprintf(file, "digraph G{\n");
    fprintf(file, "node [shape=\"record\", style=\"solid\", color=\"blue\"];\n");

    loadNodes(file, node);

    fprintf(file, "}");

    fclose(file);

    systemCall("serviceFiles/graphPlot.txt", filename);
}

void graphRepoter(Node* node, const char* reason)
{
    static size_t number_of_calls = 0;
    printf("====\nGRAPH REPORTER CALL #%zu\n======\n", number_of_calls);

    char filename[128];
    
    snprintf(filename, 128, "Nodes/%zu%s.png", number_of_calls, reason);
    drawGraph(node, filename);

    ++number_of_calls;
}


//Recording
void recordTree(Node* node, const char* filename)
{
    FILE* file = fopen(filename, "w");

    recordNode(node, file);

    fclose(file);
}

void recordNode(Node* node, FILE* file)
{
    if (node == nullptr) return;
    
    fprintf(file, "%d | ", DUMP_TYPE_TRANSALATOR[node->type]);

    switch (getNodeType(node))
    {
        case NUMBER_TYPE:
            fprintf(file, "%lg", node->value.number);
            break;

        case NAMING_TYPE:
            fprintf(file, "%s", node->value.naming.string);
            break;

        case MATH_TYPE:
            fprintf(file, "%d", DUMP_MATH_TRANSLATOR[(int)node->value.number]);
            break;
        
        default:
            fprintf(file, "0");
    }

    fprintf(file, " {");
    recordNode(node->left, file);
    fprintf(file, " } { ");
    recordNode(node->right, file);
    fprintf(file, "} ");
}