#include "../headerFiles/frontEndHeader.h"

int main()
{
    char* buffer = readFile("myProgramms/input.txt");

    TokenArray* tokens = getTokens(buffer);

    showTokens(tokens);

    Node* parsed_tree = getGeneral(tokens);

    drawGraph(parsed_tree, "serviceFiles/Graph.jpg");
    recordTree(parsed_tree, "serviceFiles/recordedTree.txt");

    destructTokenArray(tokens);
    free(buffer);   

    return 0;
}