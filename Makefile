NAME = backEnd
ADD_LIB = frontEnd
MAIN_DIR = mainFiles
OBJ_DIR = objectsFiles
COMPILATOR = g++
BIN_NAME = translator.exe


$(NAME): $(OBJ_DIR)/$(NAME)Main.o $(OBJ_DIR)/$(NAME)Funcs.o 
	$(COMPILATOR) $(OBJ_DIR)/$(NAME)Main.o $(OBJ_DIR)/$(NAME)Funcs.o  $(OBJ_DIR)/$(ADD_LIB)Funcs.o -o $(BIN_NAME)

$(OBJ_DIR)/$(ADD_LIB)Funcs.o: $(MAIN_DIR)/$(ADD_LIB)Files/$(ADD_LIB)Funcs.cpp
	$(COMPILATOR) -c $(MAIN_DIR)/$(ADD_LIB)Files/$(ADD_LIB)Funcs.cpp -o $(OBJ_DIR)/$(ADD_LIB)Funcs.o
	
$(OBJ_DIR)/$(NAME)Main.o: $(MAIN_DIR)/$(NAME)Files/$(NAME)Main.cpp
	$(COMPILATOR) -c $(MAIN_DIR)/$(NAME)Files/$(NAME)Main.cpp -o $(OBJ_DIR)/$(NAME)Main.o

$(OBJ_DIR)/$(NAME)Funcs.o: $(MAIN_DIR)/$(NAME)Files/$(NAME)Funcs.cpp
	$(COMPILATOR) -c $(MAIN_DIR)/$(NAME)Files/$(NAME)Funcs.cpp -o $(OBJ_DIR)/$(NAME)Funcs.o
