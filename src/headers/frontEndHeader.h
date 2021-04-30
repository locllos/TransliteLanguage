#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/stat.h>
#include <ctype.h>
#include <math.h>
#include "dataStructures/frontEndStructues.h"

//Service funcs
void                skipUselessSymbols  (char* string, size_t* pos, size_t* line);
bool                isValidSymbol       (char symbol);
bool                Require             (const String* string, const char* buffer, size_t* pos);
char*               readFile            (const char* filename, size_t* buffer_size = nullptr);
char*               readFile            (const char* filename, const char* flag, size_t* buffer_size = nullptr);
int                 approxLength        (const char* filename);
int                 getTokenType        (char* string, size_t* pos);
inline TokenType    getTokenType        (TokenArray* tokens, size_t* pos);
inline TokenType    getNextTokenType    (TokenArray* tokens, size_t* pos);


//Node
void                printNodeValue      (FILE* file, Node* node);
void                loadNodes           (FILE* file, Node* node);
void                drawGraph           (Node* node, const char* filename);
void                graphRepoter        (Node* node, const char* reason);
void                showNode            (Node* node);
void                printNode(Node* node);

//String
void        constructString         (String* string);
void        constructString         (String* string, char* cstring, size_t length = 0);
void        constructString         (String* string, const char* cstring, size_t length);
void        constructStringVAList   (String* string, const char* formatted_cstring, va_list begin);
void        reallocateString        (String* string);
void        stringAppend            (String* string, char symbol);
void        destructString          (String* string);
String*     newString               ();
String*     newString               (String* copy_string);
String*     newString               (char* cstring, size_t length = 0);
String*     newString               (const char* cstring, size_t length);
String*     newFormattedString      (const char* formatted_cstring, ...);
String*     newStringVAList         (const char* formatted_cstring, va_list begin);
String*     deleteString            (String* string);

//TokenArray
void        constructTokenArray (TokenArray* tokens, size_t base_size);
void        reallocateTokenArray(TokenArray* tokens);
void        tokenArrayAppend    (TokenArray* tokens, TokenType type, size_t line, Value value);
void        showTokens          (TokenArray* tokens);
void        destructTokenArray  (TokenArray* tokens);
void        showToken           (Token* token, int i = -1);
TokenArray* newTokenArray       (size_t base_size);

//Tokenization
String      getNaming           (char* buffer, size_t* pos);
double      getNumber           (char* string, size_t* pos);
TokenArray* getTokens           (char* buffer);

//Recursive descent
Node*       getGeneral              (TokenArray* tokens);
Node*       getProgramm             (TokenArray* tokens, size_t* pos);
Node*       getDefinition           (TokenArray* tokens, size_t* pos);
Node*       getFunctionDeclaration  (TokenArray* tokens, size_t* pos);
Node*       getParametrList         (TokenArray* tokens, size_t* pos);
Node*       getArgumentList         (TokenArray* tokens, size_t* pos);
Node*       getBlock                (TokenArray* tokens, size_t* pos);
Node*       getStatement            (TokenArray* tokens, size_t* pos);
Node*       getCondition            (TokenArray* tokens, size_t* pos);
Node*       getLoop                 (TokenArray* tokens, size_t* pos);
Node*       getCall                 (TokenArray* tokens, size_t* pos);
Node*       getAssign               (TokenArray* tokens, size_t* pos);
Node*       getReturn               (TokenArray* tokens, size_t* pos);
Node*       getExpression           (TokenArray* tokens, size_t* pos);
Node*       getSimple               (TokenArray* tokens, size_t* pos);
Node*       getTerm                 (TokenArray* tokens, size_t* pos);
Node*       getPrimaryExpression    (TokenArray* tokens, size_t* pos);
Node*       getBottom               (TokenArray* tokens, size_t* pos);
Node*       getNaming               (TokenArray* tokens, size_t* pos);
Node*       getNumber               (TokenArray* tokens, size_t* pos);

//Recording
void        recordTree              (Node* node, const char* filename);
void        recordNode              (Node* node, FILE* file);
