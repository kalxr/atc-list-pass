all:
	./misc/run_tests.sh

bitcode:
	./misc/bitcode.sh

bitcode_remove:
	rm -f test*/bitcode/*
	./misc/clean.sh

oracle:
	./misc/oracle.sh

oracle_remove:
	rm -f test*/output*/*

reinstall: bitcode_remove oracle_remove clean bitcode oracle

clean:
	./misc/clean.sh
