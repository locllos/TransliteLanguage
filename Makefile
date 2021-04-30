SRC_PATH = src/backend
BUILDER_PATH = src/elf64_builder
ADD_OBJ_PATH = bin/frontend_bin
OBJ_PATH = bin/backend_bin
OBJS = $(OBJ_PATH)/compilator.o $(OBJ_PATH)/function.o $(OBJ_PATH)/name_table.o $(OBJ_PATH)/service_funcs.o $(OBJ_PATH)/variable_array.o $(OBJ_PATH)/array.o $(OBJ_PATH)/builder.o $(OBJ_PATH)/label_array.o
COMPILE = gcc
FLAGS = -g3 -O0
NAME = translator

main_backend: $(OBJS) $(ADD_OBJ_PATH)/frontend_funcs.o $(OBJ_PATH)/builder.o
	$(COMPILE) $(SRC_PATH)/main_backend.cpp $(OBJS) $(ADD_OBJ_PATH)/frontend_funcs.o -o $(NAME) $(FLAGS)

$(OBJ_PATH)/builder.o: $(OBJ_PATH)/array.o
	$(COMPILE) $(BUILDER_PATH)/builder.cpp -o $(OBJ_PATH)/builder.o -c $(FLAGS)

$(OBJ_PATH)/array.o: 
	$(COMPILE) $(BUILDER_PATH)/array.cpp -o $(OBJ_PATH)/array.o -c $(FLAGS)

$(ADD_OBJ_PATH)/frontend_funcs.o:
	make -f Makefile_frontEnd $(ADD_OBJ_PATH)/frontend_funcs.o

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

$(OBJ_PATH)/label_array.o:
	$(COMPILE) $(SRC_PATH)/label_array.cpp -o $(OBJ_PATH)/label_array.o -c $(FLAGS)

all: main_backend clear
	echo -e "\033[0;32mSuccess!\033[0m"

clear: clear_frontend
	rm -r $(OBJ_PATH)/*.o

clear_frontend:
	rm $(ADD_OBJ_PATH)/frontend_funcs.o
