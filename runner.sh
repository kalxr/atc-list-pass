cd CAT_lib/misc;

filename="performance1.c"
ex=$(echo "$filename" | cut -f 1 -d '.')

clang -O3 -march=native -fdeclspec $filename CAT.c;
perf stat -e L1-dcache-loads,L1-dcache-stores,L1-dcache-load-misses ./a.out;

### APPLY TRANSFORMATION ###

# clang -O1 -emit-llvm -c -Xclang -disable-llvm-passes A.c -o A.bc
clang -O1 -S -emit-llvm -Xclang -disable-llvm-passes -fdeclspec $filename -o "$ex".ll
# clang -emit-llvm -c $filename
noelle-norm "$ex".ll -o "$ex".ll; 
noelle-load -S -load ~/CAT/lib/CAT.so -CAT "$ex".ll -o "$ex".ll; 
opt -O3 -fdeclspec "$ex".ll -o "$ex".ll;
clang -O3 -c -march=native -fdeclspec "$ex".ll;
clang -g -O3 -march=native -fdeclspec "$ex".o CAT.c;
perf stat -e L1-dcache-loads,L1-dcache-stores,L1-dcache-load-misses ./a.out;

cd ../..;
