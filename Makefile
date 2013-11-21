.PHONY: all clean

###############################################################################
# Flags                                                                       #
###############################################################################

# Directorio de la la librería googletest
GTEST_DIR = lib/gtest-1.6.0

# Flags del preprocesador C++.
CPPFLAGS += -Isrc/common -I$(GTEST_DIR)/include

# Flags del compilador C++.
CXXFLAGS += -g -Wall -Wno-comment -Wextra -std=c++0x

# Comando para generar código objeto (.o)
OBJ       = $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c

# Comando para generar los binarios de las soluciones
BIN_MAIN  = $(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

# Comando para generar los binarios de los tests
BIN_TEST  = $(BIN_MAIN) -lpthread

# Dependencias extra para los binarios de los tests
TEST_DEPS = gtest-all.o

# Binarios generados
BINS      = exacto golosa local tabu tests perf

###############################################################################
# Targets generales                                                           #
###############################################################################

all: $(BINS)

clean:
	rm -f *.o $(BINS)
	rm -f informe.pdf tex/*.pdf tex/*.aux tex/*.log tex/*.toc
	
common.o: src/common/common.cpp src/common/common.h
	$(OBJ) src/common/common.cpp

familias.o: src/familias.cpp src/familias.h src/common/common.h
	$(OBJ) src/familias.cpp

###############################################################################
# Exacto                                                                      #
###############################################################################

exacto: exacto.o exacto_main.o common.o
	$(BIN_MAIN)

exacto.o: src/exacto/exacto.cpp src/exacto/exacto.h src/common/common.h
	$(OBJ) src/exacto/exacto.cpp

exacto_main.o: src/exacto/exacto.h src/exacto/exacto_main.cpp
	$(OBJ) src/exacto/exacto_main.cpp

###############################################################################
# Golosa                                                                      #
###############################################################################

golosa: golosa.o golosa_main.o common.o
	$(BIN_MAIN)

golosa.o: src/golosa/golosa.cpp src/golosa/golosa.h src/common/common.h
	$(OBJ) src/golosa/golosa.cpp

golosa_main.o: src/golosa/golosa.h src/golosa/golosa_main.cpp
	$(OBJ) src/golosa/golosa_main.cpp

###############################################################################
# Local                                                                       #
###############################################################################

local: local.o local_main.o golosa.o common.o
	$(BIN_MAIN)

local.o: src/local/local.cpp src/local/local.h src/common/common.h
	$(OBJ) src/local/local.cpp

local_main.o: src/local/local.h src/local/local_main.cpp
	$(OBJ) src/local/local_main.cpp

###############################################################################
# Tabú                                                                        #
###############################################################################

tabu: tabu.o tabu_main.o golosa.o common.o
	$(BIN_MAIN)

tabu.o: src/tabu/tabu.cpp src/tabu/tabu.h src/common/common.h
	$(OBJ) src/tabu/tabu.cpp

tabu_main.o: src/tabu/tabu.h src/tabu/tabu_main.cpp
	$(OBJ) src/tabu/tabu_main.cpp

###############################################################################
# Tests                                                                       #
###############################################################################

tests: src/tests.cpp gtest-all.o familias.o common.o
	$(BIN_TEST)

gtest-all.o:
	$(OBJ) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc

###############################################################################
# Performance                                                                 #
###############################################################################

perf: src/perf.cpp familias.o exacto.o golosa.o local.o tabu.o common.o
	$(BIN_MAIN)

###############################################################################
# Informe                                                                       #
###############################################################################

informe.pdf: tex/*.tex
	cd tex; pdflatex -interactive=nonstopmode -halt-on-error informe.tex
	cd tex; pdflatex -interactive=nonstopmode -halt-on-error informe.tex
	cp tex/informe.pdf .
