#include "../headers/backEndHeader.h"

int main(int argC, const char* argV[])
{   
    char* buffer = readFile((argC > 1) ? argV[1] : "res/myPrograms/input.txt");

    TokenArray* tokens = getTokens(buffer);

    // showTokens(tokens);

    Node* parsed_tree = getGeneral(tokens);

    drawGraph(parsed_tree, "res/serviceFiles/Graph.jpg");

    compileMain(parsed_tree, "res/myAsmFiles/asm.txt");
    
    destructTokenArray(tokens);
    free(buffer);    
}