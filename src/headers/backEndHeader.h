#include "frontEndHeader.h"
#include "array.h"
#include <elf.h>
#include "dataStructures/backEndStructures.h"

extern FILE* compilator_logs;
const size_t DEFAULT_CAPACITY = 16; 

//Service funcs
void    logWrite            (const char* format, ...);
void    asmFileWrite        (Parser* parser, const char* format, ...);
bool    isPrintFunction     (String* name);
bool    isInputFunction     (String* name);
bool    isSqrtFunction      (String* name);
void    asmFileWrite        (Parser* parser, 
                             MovType mov_type, 
                             OperandType operand_a, OperandType operand_b, 
                             MemoryAccessType mem_access = NONE, 
                             int64_t constant = 0, int8_t offset = 0);
void    asmFileWrite        (Parser* parser,
                             CommandType cmd_type,
                             OperandType operand_a = VOID_OPERAND, OperandType operand_b = VOID_OPERAND,
                             MemoryAccessType mem_access = NONE, 
                             int64_t constant = 0, int8_t offset = 0);
void    asmFileWrite        (Parser* parser, CommandType cmd_type, Label* label);
size_t  getUpperMultiple    (size_t number, size_t divisor);

//NameTable                         
void            constructNameTable  (NameTable* table, size_t base_capacity);
void            reallocateNameTable (NameTable* table);
void            nameTableAppend     (NameTable* table, Function* function);
void            destructNameTable   (NameTable* table);
NameTable*      newNameTable        (size_t base_capacity);
NameTable*      getNameTable        (Node* node);
NameTable*      deleteNameTable     (NameTable* table);
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
String*         getArgument             (Function* function, size_t idx);
int             getVariableID           (Function* function, String* variable);
int             getArgumentID           (Function* function, String* variable);
size_t          getAmountVariables      (Function* function);
size_t          getAmountArguments      (Function* function);

//VariableArray
void            constructVariableArray  (VariableArray* variables, size_t base_capacity);
void            reallocateVariableArray (VariableArray* variables);
void            variableArrayAppend     (VariableArray* variables, String* variable);
void            destructVariableArray   (VariableArray* variables);
VariableArray*  newVariableArray        (size_t base_capacity);
int             getVariableOffset       (VariableArray* variables, String* variable);

//Parser
void constructParser(Parser* parser, const char* filename, Node* parsed_tree);
void fillOffsets    (Parser* parser);
void makeELF64      (Parser* parser, const char* filename, const char* elf_filename, const char* start_function);
void destructParser (Parser* parser);

//ElF64
void initSectionHeaders     (Parser* parser, ELF* elf);
void initElfHeader          (Parser* parser, ELF* elf);
void initTextHeader         (Parser* parser, ELF* elf);
void initShstrtabHeader     (Parser* parser, ELF* elf);
void initSymtabHeader       (Parser* parser, ELF* elf);
void initStrtabHeader       (Parser* parser, ELF* elf);
void initRelatextHeader     (Parser* parser, ELF* elf);
void destructELFContents    (Parser* parser, ELF* elf);
void initSections           (Parser* parser, ELF* elf, const char* filename, const char* start_function);
void initTextSection        (Parser* parser, ELF* elf);
void initShstrtabSection    (Parser* parser, ELF* elf);
void initSymtabSection      (Parser* parser, ELF* elf);
void initStrtrabSection     (Parser* parser, ELF* elf, const char* filename, const char* start_function);
void initRelatextSection    (Parser* parser, ELF* elf);
void mergeSectionHeaders    (Array* merged_sections, ELF* elf);
void mergeSections          (Array* merged_sections, ELF* elf);
void destructElf            (ELF* elf);

//OffsetArray (contains with LabelTable in label_table.cpp)
void constructOffsetArray   (OffsetArray* array, size_t start_capacity);
void reallocOffsetArray     (OffsetArray* array, size_t new_capacity);
void pushBackOffsetArray    (OffsetArray* array, size_t offset);
void destructOffsetArray    (OffsetArray* array);

OffsetArray* newOffsetArray     (size_t start_capacity = DEFAULT_CAPACITY);
OffsetArray* deleteOffsetArray  (OffsetArray* array);

//LabelTable
void constructLabelTable    (LabelTable* labels, size_t start_capacity);
void reallocLabelTable      (LabelTable* labels, size_t new_capacity);
void pushBackLabelTable     (LabelTable* labels, Label new_label);
void pushBackLabelTable     (LabelTable* labels, size_t label_offset, const char* formatted_name, ...);
void pushBackOffset         (LabelTable* labels, char* label_name, size_t offset);
void updateLabelOffset      (LabelTable* labels, char* label_name, size_t label_offset);
void pushBackOffset         (Label* label, size_t offset);
void setLabelOffset         (LabelTable* labels, size_t label_offset, char* label_name);
void setLabelOffset         (Label* label, size_t label_offset);
void destructLabelTable     (LabelTable* labels);
Label* newLabel             (char* label_name, size_t label_offset);
Label* putBackLabelTable    (LabelTable* labels, Label new_label);
Label* putBackLabelTable    (LabelTable* labels, size_t label_offset, const char* formatted_name, ...);
Label* deleteLabel          (Label* label);
Label* getLabel             (LabelTable* labels, char* label_name);      
LabelTable* newLabelTable   (size_t start_capacity);
LabelTable* deleteLabelTable(LabelTable* labels);

//Compiler
void            compileMain             (Node* begin_node, const char* filename, const char* elf_filename);
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

//elf64_builder
void constructStringTableSection(Array* sh_strtab, Array* strtab, 
                                 char* filename, 
                                 NameTable* name_table, Function** extern_functions);
void constructSymbolTableSection(Array* sh_symtab, Array* symtab, 
                                 NameTable* name_table, Function** extern_functions);
void constructRelaTextSection   (Array* sh_relatext, Array* relatext, 
                                 NameTable* name_table, Function** extern_functions);
void writeElf64             (Parser* parser, char* filename, char* elf64_filename);