SRC_PATH = /home/locllos/Документы/Projects/TransliteLanguage/src/backend
ADD_OBJ_PATH = /home/locllos/Документы/Projects/TransliteLanguage/bin/frontend_bin
OBJ_PATH = /home/locllos/Документы/Projects/TransliteLanguage/bin/backend_bin
OBJS = $(OBJ_PATH)/compilator.o $(OBJ_PATH)/function.o $(OBJ_PATH)/name_table.o $(OBJ_PATH)/service_funcs.o $(OBJ_PATH)/variable_array.o
COMPILE = gcc
FLAGS = -g3
NAME = translator

main_backend: $(OBJS) $(ADD_OBJ_PATH)/frontend_funcs.o
	$(COMPILE) $(SRC_PATH)/main_backend.cpp $(OBJS) $(ADD_OBJ_PATH)/frontend_funcs.o -o $(NAME) $(FLAGS)

$(SRC_PATH)/frontend_funcs.o:
	make -f Makefile_frontend $(ADD_OBJ_PATH)/frontend_funcs.o

$(OBJ_PATH)/compilator.o: 
	$(COMPILE) $(SRC_PATH)/compilator.cpp -o $(OBJ_PATH)/compilator.o -c $(FLAGS)

$(OBJ_PATH)/function.o: 
	$(COMPILE) $(SRC_PATH)/function.cpp -o $(OBJ_PATH)/function.o -c $(FLAGS)

$(OBJ_PATH)/name_table.o: 
	$(COMPILE) $(SRC_PATH)/name_table.cpp -o $(OBJ_PATH)/name_table.o -c $(FLAGS)

$(OBJ_PATH)/service_funcs.o: 
	$(COMPILE) $(SRC_PATH)/service_funcs.cpp -o $(OBJ_PATH)/service_funcs.o -c $(FLAGS)

$(OBJ_PATH)/variable_array.o:			
	$(COMPILE) $(SRC_PATH)/variable_array.cpp -o $(OBJ_PATH)/variable_array.o -c $(FLAGS)

clear:
	rm -r $(OBJ_PATH)/*.o