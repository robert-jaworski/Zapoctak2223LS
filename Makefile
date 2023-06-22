CPP_ARGS = -O2

SRC_FILES = assert dynamic_matrix matrix printing number_types numbers bigint bigint10

CPP_FILES = $(addprefix src/, $(addsuffix .cpp, $(SRC_FILES)))
HPP_FILES = $(addprefix src/, $(addsuffix .hpp, $(SRC_FILES)))
OBJ_FILES = $(addprefix build/, $(addsuffix .o, $(SRC_FILES)))

TESTS = assignment_test fixed_assignment_test equality_test addition_test multiplication_test assert_test REF_test finite_field_test fraction_test	

TESTS_EXE = $(addprefix build/, $(TESTS))

all: lib/matrices.a src/all.hpp $(TESTS_EXE)

clean:
	rm -rf build/* lib/* src/All.hpp

test: src/all.hpp $(TESTS_EXE)
	bin/run_tests.sh $(TESTS_EXE)

.PHONY: all clean test

build/%.o: src/%.cpp $(HPP_FILES)
	g++ -c $(CPP_ARGS) $< -o $@

lib/matrices.a: $(OBJ_FILES)
	ar rvf $@ $^

src/all.hpp: $(HPP_FILES)
	echo '#pragma once' >$@
	bin/make_all_hpp.sh $(addsuffix .hpp, $(SRC_FILES)) >>$@

build/%: test/%.cpp build/test_wrapper.o lib/matrices.a
	g++ $(CPP_ARGS) $^ -o $@

build/test_wrapper.o: test/test_wrapper.cpp
	g++ -c $(CPP_ARGS) $^ -o $@
