#include "../headers/backEndHeader.h"

int main(int argC, const char* argV[])
{   
    char* buffer = readFile((argC > 1) ? argV[1] : "res/myPrograms/fact.txt");

    TokenArray* tokens = getTokens(buffer);

    Node* parsed_tree = getGeneral(tokens);

    drawGraph(parsed_tree, "res/serviceFiles/Graph.jpg");

    compileMain(parsed_tree, "res/myAsmFiles/asm.nasm", "my_elf64.o");
    
    destructTokenArray(tokens);
    free(buffer);  

    // asmFileWrite(nullptr, SUB, RCX, REGISTER_ONLY);
    // size_t buffer_size = 0; 
    // char* buffer = readFile("res/serviceFiles/a.out", "rb", &buffer_size);

    // Parser parser = {};
    // parser.asm_file = fopen("res/serviceFiles/asm.nasm", "wb");
    // parser.text = newArray(16, DEFAULT_REALLOC, 2);
    // //////////////////////////////////////////////
    
    // asmFileWrite(&parser, PUSH, RSP);
    // asmFileWrite(&parser, XCHG, RAX, RBX);
    // asmFileWrite(&parser, XOR, RAX, RAX);
    // asmFileWrite(&parser, ADD_REGS, RAX, RBX);
    // asmFileWrite(&parser, ADD_CONST32, R13, CONST32, NONE, 0x11223344);
    // asmFileWrite(&parser, SUB_REGS, RCX, RBX);
    // asmFileWrite(&parser, SUB_CONST32, R12, CONST32, NONE, 0x11223344);
    // asmFileWrite(&parser, IDIV, RCX);
    // asmFileWrite(&parser, CMP, RAX, RBX);
    // asmFileWrite(&parser, CMP, RBX, RCX);
    // asmFileWrite(&parser, IMUL, R13, RCX);
    
    // Label* function = newLabel("function", 0);
    // asmFileWrite(&parser, CALL, function);
    // asmFileWrite(&parser, RET);
    // asmFileWrite(&parser, JMP, function);
    // asmFileWrite(&parser, JNE, function);
    // asmFileWrite(&parser, JE, function);
    // asmFileWrite(&parser, JLE, function);
    // asmFileWrite(&parser, JL, function);
    // asmFileWrite(&parser, JGE, function);
    // asmFileWrite(&parser, JG, function);
    // asmFileWrite(&parser, SYSCALL);
    // asmFileWrite(&parser, LEAVE);

    // //////////////////////////////////////////////
    // memcpy(buffer + 0x1000, parser.text->data, parser.text->size);
    // FILE* update_exec = fopen("modified.out", "wb");

    // fwrite(buffer, sizeof(char), buffer_size, update_exec);

    // fclose(update_exec);
    // fclose(parser.asm_file);
    // parser.text = deleteArray(parser.text);



    // char* buffer = readFile((argC > 1) ? argV[1] : "res/myPrograms/fact.txt");
    // elem_t data[] = {(char)0xB8, (char)0x02, (char)0x00, (char)0x00, (char)0x00, (char)0xBB, (char)0x03, (char)0x00,
    //                  (char)0x00, (char)0x00, (char)0x48, (char)0x01, (char)0xD8, (char)0xB8, (char)0x3C, (char)0x00,
    //                  (char)0x00, (char)0x00, (char)0x0F, (char)0x05};
    // elem_t data[] = {
    //     (char)0x48, (char)0x83, (char)0xEC, (char)0x08, (char)0xE8, (char)0x0, (char)0x00, (char)0x00,
    //     (char)0x00, (char)0x48, (char)0x83, (char)0xC4, (char)0x08, (char)0x48, (char)0x89, (char)0xC7,
    //     (char)0xE8, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0xB8, (char)0x3C};
    // Parser parser = {};
    // parser.text = newArray(16, LINEAR_REALLOC, 16);
    // parser.table = newNameTable(1);

    // char* filename = (char*)calloc(128, sizeof(char));
    // char* elf64_filename = (char*)calloc(128, sizeof(char));

    // strcpy(filename, "my_elf64.cpp");
    // strcpy(elf64_filename, "my_elf64.o");
    // insertBackArray(parser.text, data, sizeof(data));

    // writeElf64(&parser, filename, elf64_filename);
    // TokenArray* tokens = getTokens(buffer);

    // showTokens(tokens);

    // Node* parsed_tree = getGeneral(tokens);

    // drawGraph(parsed_tree, "res/serviceFiles/Graph.jpg");

    // compileMain(parsed_tree, "res/myAsmFiles/asm.nasm");
    
    // destructTokenArray(tokens);
    // free(buffer);    
}