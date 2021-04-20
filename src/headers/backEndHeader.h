#include "frontEndHeader.h"
#include "dataStructures/backEndStructures.h"

extern FILE* compilator_logs;

//Service funcs
void     logWrite            (const char* format, ...);
void     asmFileWrite        (Parser* parser, const char* format, ...);
bool     isPrintFunction     (String* name);
bool     isInputFunction     (String* name);
bool     isSqrtFunction      (String* name);

//NameTable
void            constructNameTable  (NameTable* table, size_t base_capacity);
void            reallocateNameTable (NameTable* table);
void            nameTableAppend     (NameTable* table, Function* function);
void            destructNameTable   (NameTable* table);
NameTable*      newNameTable        (size_t base_capacity);
NameTable*      getNameTable        (Node* node);
Function*       getFunction         (NameTable* table, String* name);
Function*       getFunction         (NameTable* table, size_t func_id);
int             getFunctionID       (NameTable* table, String* name);

//Function
void            constructFunction       (Function* function, String* name);
void            desctructFunction       (Function* function);
void            recordArguments         (Function* function, Node* naming_node);
void            recordLocalVariables    (Function* function, Node* node);
Function*       newFunction             (String* name);
Function*       getCurrentFunction      (Parser* parser);
String*         getVariable             (Function* function, size_t idx);
int             getVariableID           (Function* function, String* variable);
size_t          getAmountVariables      (Function* function);

//VariableArray
void            constructVariableArray  (VariableArray* variables, size_t base_capacity);
void            reallocateVariableArray (VariableArray* variables);
void            variableArrayAppend     (VariableArray* variables, String* variable);
void            destructVariableArray   (VariableArray* variables);
VariableArray*  newVariableArray        (size_t base_capacity);
int             getVariableOffset       (VariableArray* variables, String* variable);

//Compilator
void            compileMain             (Node* begin_node, const char* filename);
void            compileDefinition       (Node* node, Parser* parser);
void            compileEntry            (Parser* parser);
void            compileExit             (Parser* parser);
void            compileVariables        (Node* node, Parser* parser);
void            compileBlock            (Node* node, Parser* parser);
void            compileStatement        (Node* node, Parser* parser);
void            compileCondition        (Node* node, Parser* parser);
void            compileLoop             (Node* node, Parser* parser);
void            compileReturn           (Node* node, Parser* parser);
void            compileAssign           (Node* node, Parser* parser);
void            compileExpression       (Node* node, Parser* parser);
void            compileMath             (Node* node, Parser* parser);
void            compileCall             (Node* node, Parser* parser);
void            compileNumber           (Node* node, Parser* parser);
void            compileVariable         (Node* node, Parser* parser);
void            compileLogicOperators   (Node* node, Parser* parser);
void            compilePrint            (Node* node, Parser* parser);
void            compileInput            (Node* node, Parser* parser);
void            compilePrint            (Node* node, Parser* parser);
void            compileInput            (Node* node, Parser* parser);
void            compileSqrt             (Node* node, Parser* parser);
