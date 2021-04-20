#include "headerFiles/frontEndHeader.h"
#include "frontEndConstants.h"

int approxLength(const char* filename)
{
	assert(filename);
	
	struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

	stat(filename, buff);

	return buff->st_size;
}

void skipUselessSymbols(char* string, size_t* pos)
{   
    size_t i = *pos;
    while (isspace(string[i]))
    {   
        ++i;
    }
    *pos = i;
}

char* readFile(const char* filename, size_t* buffer_size = nullptr)
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

void initString(String* string, char* buffer, size_t length, size_t capacity)
{
    string->string = buffer;
    string->length = length;
    string->capacity = capacity;
}

void constructNode(Node* node, Type type)
{
    node->left = nullptr;
    node->right = nullptr;
    node->type = type;
    node->value = {};
}

Node* newNode(Type type)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    constructNode(new_node, type);

    return new_node;
}

size_t valueLength(char* buffer)
{   
    char* offset = strchr(buffer, '|');

    assert(offset);

    return offset - buffer;
}

void scanNumber(Node* node, char* buffer, size_t* pos)
{
    size_t offset = 0;

    sscanf(buffer + *pos, "%lg%n", &node->value.number, &offset);
    
    *pos += offset + 1;    
}

void scanNaming(Node* node, char* buffer, size_t* pos)
{
    char* naming = strchr(buffer + *pos, '|');
    size_t length = naming - buffer - 1;

    *pos += length + 1;
    buffer[*pos - 1] = '\0';

    initString(&node->value.naming, naming, length, length);
}

void scanMath(Node* node, char* buffer, size_t* pos)
{
    size_t offset = 0;
    size_t math_type = 0;

    sscanf(buffer + *pos, "%zu%n", &math_type, &offset);

    node->value.number = MATH_TRANSLATOR[math_type];
    
    *pos += offset + 1; 
}


Node* buildNodes(char* buffer, size_t* pos)
{   
    if (buffer[*pos] == '\0') return nullptr;
    skipUselessSymbols(buffer, pos);

    size_t size = 0;
    size_t type = 0;
    size_t offset = 0;
    sscanf(buffer + *pos, "%zu%n", &type, &offset);
    *pos += offset + 3;

    Node* node = newNode((Type)TYPE_TRANSALATOR[type]);
    switch (node->type)
    {
        case NAMING_TYPE:
            scanNaming(node, buffer, pos);
            break;

        case NUMBER_TYPE:
            scanNumber(node, buffer, pos);
            break;

        case MATH_TYPE:
            scanMath(node, buffer, pos);
            break;

        default:
            node->value.number = 0;
            *pos += 2;
            break;
    }

    skipUselessSymbols(buffer, pos);
    if (buffer[*pos] == '{')
    {
        *pos += 1;
        node->left = buildNodes(buffer, pos);
    }
    skipUselessSymbols(buffer, pos);
    if (buffer[*pos] == '}')
    {
        *pos += 1;
        node->left = buildNodes(buffer, pos);
    }
}

void buildTree(const char* filename)
{
    FILE* file = fopen(filename, "r");

    char* buffer = readFile(filename);

    size_t pos = 0;
    Node* root = buildNodes(buffer, &pos);

}


int main()
{
    return 0;
}