#include "../headers/frontEndHeader.h"

int main()
{
    char* buffer = readFile("res/myPrograms/input.txt");

    TokenArray* tokens = getTokens(buffer);

    showTokens(tokens);

    Node* parsed_tree = getGeneral(tokens);

    drawGraph(parsed_tree, "res/serviceFiles/Graph.jpg");
    recordTree(parsed_tree, "res/serviceFiles/recordedTree.txt");

    destructTokenArray(tokens);
    free(buffer); 

    return 0;
}