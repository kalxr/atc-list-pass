OPTIONS=-I../misc

all: compiler_output program.ll

compiler_output: program.bc ../misc/CAT.o
	cat-c $^ 2> $@

program.bc: program.c
	clang $(OPTIONS) -O1 -emit-llvm -c $<

program.ll: program.bc
	llvm-dis $^

program: program.bc
	clang $(OPTIONS) $^ -o $@

check: compiler_output
	../misc/check.sh

../misc/CAT.o: ../misc/CAT.c
	clang -O3 -c $^ -o $@

install: program.bc program.ll
	cp *.bc *.ll bitcode/

oracle: compiler_output
	mv $^ output/oracle_output

clean:
	rm -f program *.bc *.ll a.out compiler_output diff_output *.gch diff/*;
	if test -d bitcode ; then cp bitcode/* . ; fi ;

.PHONY: run oracle check clean install
