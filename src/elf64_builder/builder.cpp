#include "../headers/backEndHeader.h"
#include "constants.h"

bool isExtern(Label* label)
{
    return label->label_offset == NOT_INIT;
}

bool isStartFunctionIdx(size_t idx)
{
    return idx == IDX_START_FUNCTION;
}

bool isDefault(ELF* elf)
{
    return (elf->elf_header->e_shnum == DEFAULT_SH_AMOUNT);
}

bool isExtendedELF(Parser* parser)
{
    return parser->amount_externs != 0;
}

void initElfHeader(Parser* parser, ELF* elf)
{
    elf->elf_header = (Elf64_Ehdr*)calloc(1, sizeof(Elf64_Ehdr));
    memcpy(elf->elf_header, DEFAULT_ELF_HEADER, sizeof(Elf64_Ehdr));
    if (!isExtendedELF(parser))
    {
        elf->elf_header->e_shnum = DEFAULT_SH_AMOUNT;
    }
    else 
    {
        elf->elf_header->e_shnum = EXTENDED_SH_AMOUNT;
    }
}

void initTextHeader(Parser* parser, ELF* elf)
{
    elf->sh_text = (Elf64_Shdr*)calloc(1, sizeof(Elf64_Shdr));
    elf->sh_text->sh_name = TEXT;
    elf->sh_text->sh_type = SHT_PROGBITS;
    elf->sh_text->sh_flags = SHF_ALLOC | SHF_EXECINSTR;
    elf->sh_text->sh_addr = 0x00;
    elf->sh_text->sh_offset = (elf->elf_header->e_shnum + 1) * 0x40; 
    printf("text size: %x\n", parser->text->size);
    elf->sh_text->sh_size = parser->text->size;
    elf->sh_text->sh_link = 0x00;
    elf->sh_text->sh_info = 0x00;
    elf->sh_text->sh_addralign = 0x10;
    elf->sh_text->sh_entsize = 0x00;
}

void initShstrtabHeader(Parser* parser, ELF* elf)
{
    elf->sh_shstrtab = (Elf64_Shdr*)calloc(1, sizeof(Elf64_Shdr));
    elf->sh_shstrtab->sh_name = SHSTRTAB;
    elf->sh_shstrtab->sh_type = SHT_STRTAB;
    elf->sh_shstrtab->sh_flags = 0x00;
    elf->sh_shstrtab->sh_addr = 0x00;
    elf->sh_shstrtab->sh_offset = elf->sh_text->sh_offset + 
                                  getUpperMultiple(elf->sh_text->sh_size, DEFAULT_ALIGNMENT); 
    printf("Upper Multiple text size: %x\n", getUpperMultiple(elf->sh_text->sh_size, DEFAULT_ALIGNMENT));
    elf->sh_shstrtab->sh_size = isDefault(elf) ? DEFAULT_SHSTRTAB_SIZE : EXTENDED_SHSTRTAB_SIZE;
    elf->sh_shstrtab->sh_link = 0x00;
    elf->sh_shstrtab->sh_info = 0x00;
    elf->sh_shstrtab->sh_addralign = 0x01;
    elf->sh_shstrtab->sh_entsize = 0x00;
}

void initSymtabHeader(Parser* parser, ELF* elf)
{
    elf->sh_symtab = (Elf64_Shdr*)calloc(1, sizeof(Elf64_Shdr));
    elf->sh_symtab->sh_name = SYMTAB;
    elf->sh_symtab->sh_type = SHT_SYMTAB;
    elf->sh_symtab->sh_flags = 0x00;
    elf->sh_symtab->sh_addr = 0x00;
    elf->sh_symtab->sh_offset = elf->sh_shstrtab->sh_offset;
    elf->sh_symtab->sh_offset += isDefault(elf) ? sizeof(DEFAULT_SHSTRTAB) : sizeof(EXTENDED_SHSTRTAB); 
    elf->sh_symtab->sh_size = 0x00; // WILL BE FILLED in initSymtabSection 
    elf->sh_symtab->sh_link = 0x04; // link to strtab
    // idx to next not local name in symtab entry
    elf->sh_symtab->sh_info = 0x00; // WILL BE FILLED in initSymtabSection 
    elf->sh_symtab->sh_addralign = 0x08;
    elf->sh_symtab->sh_entsize = ENTRY_SIZE;
}

void initStrtabHeader(Parser* parser, ELF* elf)
{
    elf->sh_strtab = (Elf64_Shdr*)calloc(1, sizeof(Elf64_Shdr));
    elf->sh_strtab->sh_name = STRTAB;
    elf->sh_strtab->sh_type = SHT_STRTAB;
    elf->sh_strtab->sh_flags = 0x00;
    elf->sh_strtab->sh_addr = 0x00;
    elf->sh_strtab->sh_offset = 0x00;
    elf->sh_strtab->sh_size = 0x00; //WILL BE FILLED in initStrtabSection
    elf->sh_strtab->sh_link = 0x00;
    elf->sh_strtab->sh_info = 0x00;
    elf->sh_strtab->sh_addralign = 0x01;
    elf->sh_strtab->sh_entsize = 0x00;
}

void initRelatextHeader(Parser* parser, ELF* elf)
{
    elf->sh_relatext = (Elf64_Shdr*)calloc(1, sizeof(Elf64_Shdr));
    elf->sh_relatext->sh_name = RELATEXT;
    elf->sh_relatext->sh_type = SHT_RELA;
    elf->sh_relatext->sh_flags = 0x00;
    elf->sh_relatext->sh_addr = 0x00;
    elf->sh_relatext->sh_offset = 0x00; //WILL BE FILLED in initSectionHeaders
    elf->sh_relatext->sh_size = 0x00; //WILL BE FILLED in initRelatextSection
    elf->sh_relatext->sh_link = 0x03; // откуда берём доп.информацию
    elf->sh_relatext->sh_info = 0x01; // где мы применяем реалокацию
    elf->sh_relatext->sh_addralign = 0x08;
    elf->sh_relatext->sh_entsize = ENTRY_SIZE;
}

void initSectionHeaders(Parser* parser, ELF* elf)
{
    initElfHeader(parser, elf);
    initTextHeader(parser, elf);
    initShstrtabHeader(parser, elf);
    initSymtabHeader(parser, elf);
    initStrtabHeader(parser, elf);
    
    if (!isDefault(elf))
    {
        initRelatextHeader(parser, elf);
    }
}

void initTextSection(Parser* parser, ELF* elf)
{
    alignCapacityArrayBySize(parser->text, DEFAULT_ALIGNMENT);

    elf->text = parser->text;
}

void initShstrtabSection(Parser* parser, ELF* elf)
{   
    Array* shstrtab = newArray();

    insertBackArray(shstrtab, isDefault(elf) ? DEFAULT_SHSTRTAB : EXTENDED_SHSTRTAB, 
                              isDefault(elf) ? sizeof(DEFAULT_SHSTRTAB) : sizeof(EXTENDED_SHSTRTAB));

    elf->shstrtab = shstrtab;
    alignCapacityArrayBySize(shstrtab, DEFAULT_ALIGNMENT);
}

void initStrtrabSection(Parser* parser, ELF* elf, const char* filename, const char* start_function)
{
    Array* strtab = newArray();

    pushBackArray(strtab, 0);
    insertBackArray(strtab, (char*)filename, strlen(filename) + 1);
    
    for (size_t i = 0; i < parser->labels->size; ++i)
    {   
        if (isExtern(&parser->labels->data[i]) && 
                      parser->labels->data[i].offsets->size == 0)
        {
            continue;
        }
        parser->labels->data[i].strtab_offset = strtab->size;
        insertBackArray(strtab, parser->labels->data[i].name->string,
                        parser->labels->data[i].name->length + 1);
    }
    elf->main_func_strtab_offset = strtab->size;
    insertBackArray(strtab, (char*)start_function, strlen(start_function) + 1);

    elf->sh_strtab->sh_size = strtab->size;
    elf->strtab = strtab;

    alignCapacityArrayBySize(strtab, DEFAULT_ALIGNMENT);
}

void initFileEntry(Elf64_Sym* file_entry)
{
    file_entry->st_name = 1; //strtab offset is always 1 for file entry
    file_entry->st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    file_entry->st_other = STV_DEFAULT;
    file_entry->st_shndx = ABS_VALUE;
    file_entry->st_value = 0x00;
    file_entry->st_size = 0x00;
}

void initTextSectionEntry(Elf64_Sym* text_entry)
{
    text_entry->st_name = 0;
    text_entry->st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION);
    text_entry->st_other = STV_DEFAULT;
    text_entry->st_shndx = TEXT;
    text_entry->st_value = 0x00;
    text_entry->st_size = 0x00;
}

void initLocalNames(Parser* parser, ELF* elf, Array* symtab, size_t* symtab_idx)
{   
    Elf64_Sym sym_entry = {};
    for (size_t i = 0; i < parser->labels->size; ++i)
    {   
        Label* label = &parser->labels->data[i];
        
        if (isExtern(label)) continue;
        printf("%zu: %s\n", *symtab_idx, label->name->string);

        sym_entry = {label->strtab_offset, 
                     ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE),
                     STV_DEFAULT,
                     TEXT, 
                     label->label_offset + 1, 
                     0x00};
        insertBackArray(symtab, (char*)&sym_entry, sizeof(Elf64_Sym));
        label->symtab_idx = *symtab_idx;
        ++(*symtab_idx);
    }
}

void initGlobalNames(Parser* parser, ELF* elf, Array* symtab, size_t* symtab_idx)
{
    Elf64_Sym sym_entry = {};
    for (size_t i = 0; i < parser->labels->size; ++i)
    {   
        Label* label = &parser->labels->data[i];
        if (isExtern(label) && label->offsets->size != 0)
        {
            printf("%zu: %s\n", *symtab_idx, label->name->string);

            sym_entry = {label->strtab_offset, 
                        ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE),
                        STV_DEFAULT,
                        0, 
                        0, 
                        0x00};
            insertBackArray(symtab, (char*)&sym_entry, sizeof(Elf64_Sym));
            label->symtab_idx = *symtab_idx;
            ++(*symtab_idx);
        }
    }
}

void initStartFunction(Elf64_Sym* sym_entry, ELF* elf)
{   
    *sym_entry = {elf->main_func_strtab_offset, 
                  ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE),
                  STV_DEFAULT,
                  TEXT, 
                  0, 
                  0x00};
}

void initSymtabSection(Parser* parser, ELF* elf)
{
    Array* symtab = newArray();
    Elf64_Sym sym_entry = {};

    insertBackArray(symtab, ZERO_ENTRY, ENTRY_SIZE);

    initFileEntry(&sym_entry);
    insertBackArray(symtab, (char*)&sym_entry, ENTRY_SIZE);

    initTextSectionEntry(&sym_entry);
    insertBackArray(symtab, (char*)&sym_entry, ENTRY_SIZE);

    size_t symtab_idx = 3;
    initLocalNames(parser, elf, symtab, &symtab_idx);

    initStartFunction(&sym_entry, elf);
    insertBackArray(symtab, (char*)&sym_entry, ENTRY_SIZE);
    elf->sh_symtab->sh_info = symtab_idx;

    ++symtab_idx;

    initGlobalNames(parser, elf, symtab, &symtab_idx);

    elf->sh_symtab->sh_size = symtab->size;
    elf->symtab = symtab;

    alignCapacityArrayBySize(symtab, DEFAULT_ALIGNMENT);
    
}

void initRelatextSection(Parser* parser, ELF* elf)
{
    if (parser->amount_externs == 0) elf->relatext = nullptr;

    Array* relatext = newArray();
    Elf64_Rela rel_entry {};

    for (size_t i = 0; i < parser->labels->size; ++i)
    {   
        Label* label = &parser->labels->data[i];
        
        if (!isExtern(label)) continue;
        
        OffsetArray* offsets = label->offsets;
        for (size_t j = 0; j < offsets->size; ++j)
        {
            rel_entry = {offsets->data[j] - 3, ELF64_R_INFO(label->symtab_idx, 2), -4};
            insertBackArray(relatext, (char*)&rel_entry, sizeof(Elf64_Rela));
        }
    }
    elf->sh_relatext->sh_size = relatext->size;
    elf->relatext = relatext;

    alignCapacityArrayBySize(relatext, DEFAULT_ALIGNMENT);
}

void initSections(Parser* parser, ELF* elf, const char* filename, const char* start_function)
{
    initTextSection(parser, elf);
    initShstrtabSection(parser, elf);
    initStrtrabSection(parser, elf, filename, start_function);
    initSymtabSection(parser, elf);

    elf->sh_strtab->sh_offset = elf->sh_symtab->sh_offset + elf->symtab->capacity;
    
    if (!isDefault(elf))
    {
        initRelatextSection(parser, elf);
        elf->sh_relatext->sh_offset = elf->sh_strtab->sh_offset + elf->strtab->capacity;
    }
}

void mergeSectionHeaders(Array* merged_sections, ELF* elf)
{
    insertBackArray(merged_sections, (char*)elf->elf_header, sizeof(Elf64_Ehdr));
    insertBackArray(merged_sections, (char*)SH_ZERO_INDEX, sizeof(Elf64_Shdr));
    insertBackArray(merged_sections, (char*)elf->sh_text, sizeof(Elf64_Shdr));
    insertBackArray(merged_sections, (char*)elf->sh_shstrtab, sizeof(Elf64_Shdr));
    insertBackArray(merged_sections, (char*)elf->sh_symtab, sizeof(Elf64_Shdr));
    insertBackArray(merged_sections, (char*)elf->sh_strtab, sizeof(Elf64_Shdr));
    if (!isDefault(elf))
    {
        insertBackArray(merged_sections, (char*)elf->sh_relatext, sizeof(Elf64_Shdr));
    }
}

void mergeSections(Array* merged_sections, ELF* elf)
{
    insertBackArray(merged_sections, elf->text->data, elf->text->capacity);
    insertBackArray(merged_sections, elf->shstrtab->data, elf->shstrtab->capacity);
    insertBackArray(merged_sections, elf->symtab->data, elf->symtab->capacity);
    insertBackArray(merged_sections, elf->strtab->data, elf->strtab->capacity);
    if (!isDefault(elf))
    {
        insertBackArray(merged_sections, elf->relatext->data, elf->relatext->capacity);
    }
}

void destructElf(ELF* elf)
{   
    if (!isDefault(elf))
    {
        free(elf->sh_relatext);
        destructArray(elf->relatext);
    }
    free(elf->elf_header);
    free(elf->sh_text);
    free(elf->sh_shstrtab);
    free(elf->sh_symtab);
    free(elf->sh_strtab);

    destructArray(elf->shstrtab);
    destructArray(elf->symtab);
    destructArray(elf->strtab);
}

void makeELF64(Parser* parser, const char* filename, const char* elf_filename, const char* start_function)
{
    ELF elf = {};

    initSectionHeaders(parser, &elf);
    initSections(parser, &elf, filename, start_function);

    Array* merged_sections = newArray();

    mergeSectionHeaders(merged_sections, &elf);
    mergeSections(merged_sections, &elf);

    FILE* elf64 = fopen(elf_filename, "wb");
    fwrite(merged_sections->data, sizeof(char), merged_sections->size, elf64);
    fclose(elf64);

    destructElf(&elf);
    merged_sections = deleteArray(merged_sections);   
}