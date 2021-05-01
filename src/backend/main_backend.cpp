#include "../headers/backEndHeader.h"

int main()
{   
    char* buffer = readFile("res/myPrograms/fact.txt");

    TokenArray* tokens = getTokens(buffer);

    Node* parsed_tree = getGeneral(tokens);

    drawGraph(parsed_tree, "res/serviceFiles/Graph.jpg");

    compileMain(parsed_tree, "res/myAsmFiles/asm.nasm", "my_elf64.o");
    
    destructTokenArray(tokens);
    free(buffer);     
}