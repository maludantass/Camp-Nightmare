# Nome do executável principal
TARGET = game

# Compilador e flags
CC = gcc
CFLAGS = -Wall -std=c99

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include

# Lista de arquivos .c específicos
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/menu.c $(SRC_DIR)/map.c $(SRC_DIR)/keyboard.c $(SRC_DIR)/screen.c $(SRC_DIR)/timer.c $(SRC_DIR)/medio.c

# Bibliotecas necessárias (adicione aqui se necessário)
LIBS =  -lpthread

# Regra para gerar o executável principal
$(TARGET): $(SOURCES)
	@echo "Iniciando a compilação..."
	$(CC) $(CFLAGS) $(SOURCES) -I$(INCLUDE_DIR) -o $(TARGET) $(LIBS)

# Limpeza dos arquivos temporários e executáveis
clean:
	rm -f $(TARGET)
