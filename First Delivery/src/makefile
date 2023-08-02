# Variáveis utilizadas no Makefile

CC = gcc # Compilador usado
CFLAGS = -std=c99 -Wall -Werror # Flags de compilação
OBJECTS = funcoesFornecidas.o funcionalidades.o csv.o binario.o main.o # Arquivos necessários
VALG_FLAGS = --track-origins=yes --leak-check=full --show-leak-kinds=all # Flags do valgrind


# Regra para compilação do arquivo executável
all: $(OBJECTS)
	$(CC) $(OBJECTS) -o main $(CFLAGS)


# Regra geral para gerar .o dado um .c
%.o: %.c
	$(CC) -c $<


# Excluir objetos e executável
clean:
	rm *.o main 


# Executar programa
run:
	./main