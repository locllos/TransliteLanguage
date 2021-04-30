#include "../headers/backEndHeader.h"
#include "constants.h"

//OffsetArray
void constructOffsetArray(OffsetArray* array, size_t start_capacity)
{
    array->data = (size_t*)calloc(start_capacity, sizeof(size_t));
    array->capacity = start_capacity;
    array->size = 0;
}

void reallocOffsetArray(OffsetArray* array, size_t new_capacity)
{
    array->data = (size_t*)realloc(array->data, new_capacity * sizeof(size_t));
    array->capacity = new_capacity;
}

void pushBackOffsetArray(OffsetArray* array, size_t offset)
{
    if (array->size + 1 > array->capacity)
    {
        reallocOffsetArray(array, array->capacity * 2);
    }
    array->data[array->size++] = offset;
}

void destructOffsetArray(OffsetArray* array)
{
    free(array->data);

    array->data = nullptr;
    array->capacity = 0;
    array->size = 0;
}

OffsetArray* newOffsetArray(size_t start_capacity)
{
    OffsetArray* array = (OffsetArray*)calloc(1, sizeof(OffsetArray));

    constructOffsetArray(array, start_capacity);

    return array;
}

OffsetArray* deleteOffsetArray(OffsetArray* array)
{
    destructOffsetArray(array);

    free(array);

    return nullptr;
}

//LabelTable
void constructLabelTable(LabelTable* labels, size_t start_capacity)
{
    labels->data = (Label*)calloc(start_capacity, sizeof(Label));

    labels->capacity = start_capacity;
    labels->size = 0;
}

void reallocLabelTable(LabelTable* labels, size_t new_capacity)
{
    labels->data = (Label*)realloc(labels->data, new_capacity * sizeof(Label));
    labels->capacity = new_capacity;
}

void pushBackLabelTable(LabelTable* labels, Label new_label)
{
    Label* useless = putBackLabelTable(labels, new_label);
}

void pushBackLabelTable(LabelTable* labels, size_t label_offset, const char* formatted_name, ...)
{
    if (labels->size + 1 > labels->capacity)
    {
        reallocLabelTable(labels, labels->capacity * 2);
    }
    va_list begin;
    va_start(begin, formatted_name);    

    labels->data[labels->size++] = {newStringVAList(formatted_name, begin), 
                                  label_offset, 0, 0, 
                                  newOffsetArray(START_OFFSET_ARRAY_CAPACITY)};

    va_end(begin);
}

void pushBackOffset(LabelTable* labels, char* label_name, size_t offset)
{
    Label* label = getLabel(labels, label_name);

    assert(label);

    pushBackOffsetArray(label->offsets, offset);
}

void updateLabelOffset(LabelTable* labels, char* label_name, size_t label_offset)
{
    Label* label = getLabel(labels, label_name);

    if (label == nullptr) 
    {
        pushBackLabelTable(labels, {newString(label_name), label_offset, 0, 0, newOffsetArray()});
        return;
    }
    label->label_offset = label_offset;
}

void pushBackOffset(Label* label, size_t offset)
{
    pushBackOffsetArray(label->offsets, offset);
}

void setLabelOffset(LabelTable* labels, size_t label_offset, char* label_name)
{
    Label* label = getLabel(labels, label_name);

    label->label_offset = label_offset;
}

void setLabelOffset(Label* label, size_t label_offset)
{
    label->label_offset = label_offset;
}

void destructLabelTable(LabelTable* labels)
{
    for (size_t i = 0; i < labels->size; ++i)
    {
        destructOffsetArray(labels->data[i].offsets);
        labels->data[i].name = deleteString(labels->data[i].name);
    }
    free(labels->data);
    labels->data = nullptr;
    labels->capacity = 0;
    labels->size = 0;
}

Label* putBackLabelTable(LabelTable* labels, Label new_label)
{
    if (labels->size + 1 > labels->capacity)
    {
        reallocLabelTable(labels, labels->capacity * 2);
    }
    labels->data[labels->size++] = new_label;

    return &labels->data[labels->size - 1];
}

Label* putBackLabelTable(LabelTable* labels, size_t label_offset, const char* formatted_name, ...)
{
    if (labels->size + 1 > labels->capacity)
    {
        reallocLabelTable(labels, labels->capacity * 2);
    }
    va_list begin;
    va_start(begin, formatted_name);    

    labels->data[labels->size++] = {newStringVAList(formatted_name, begin), 
                                    label_offset, 0, 0, 
                                    newOffsetArray(START_OFFSET_ARRAY_CAPACITY)};

    va_end(begin);

    return &labels->data[labels->size - 1];
}

LabelTable* newLabelTable(size_t start_capacity)
{
    LabelTable* labels = (LabelTable*)calloc(1, sizeof(LabelTable));

    constructLabelTable(labels, start_capacity);

    return labels;
}

Label* getLabel(LabelTable* labels, char* label_name)
{
    size_t size = labels->size;
    for (size_t i = 0; i < size; ++i)
    {
        if (strcmp(label_name, labels->data[i].name->string) == 0)
        {
            return &labels->data[i];
        }
    }
    return nullptr;
}

Label* newLabel(char* label_name, size_t label_offset)
{
    Label* label = (Label*)calloc(1, sizeof(Label));

    label->offsets = newOffsetArray(DEFAULT_START_CAPACITY);
    label->name = newString(label_name);
    label->label_offset = label_offset;

    return label;
}

Label* deleteLabel(Label* label)
{
    label->name = deleteString(label->name);
    label->offsets = deleteOffsetArray(label->offsets);
    label->label_offset = 0;

    free(label);

    return nullptr;
}

LabelTable* deleteLabelTable(LabelTable* labels)
{
    destructLabelTable(labels);
    
    free(labels);

    return nullptr;
}
