cd CAT_lib/misc/performance;

filename="performance1.c"
ex=$(echo "$filename" | cut -f 1 -d '.')

clang -O3 -Rpass-analysis=loop-vectorize -march=native -fdeclspec $filename ../CAT.c;
perf stat -r 1 -e L1-dcache-loads,L1-dcache-stores,L1-dcache-load-misses,instructions,branch-misses ./a.out;

### APPLY TRANSFORMATION ###

# clang -O1 -emit-llvm -c -Xclang -disable-llvm-passes A.c -o A.bc
clang -O1 -S -Rpass-analysis=loop-vectorize -emit-llvm -Xclang -disable-llvm-passes -fdeclspec $filename -o "$ex".ll
# clang -emit-llvm -c $filename
noelle-norm "$ex".ll -o "$ex".ll; 
noelle-load -S -load ~/CAT/lib/CAT.so -CAT "$ex".ll -o "$ex".ll;

#opt -O3 "$ex".ll -o "$ex".ll;
clang -O3 -S -Rpass-analysis=loop-vectorize -emit-llvm "$ex".ll -o "$ex".ll;
clang -O3 -c -Rpass-analysis=loop-vectorize -march=native -fdeclspec "$ex".ll;
clang -g -O3 -Rpass-analysis=loop-vectorize -march=native -fdeclspec "$ex".o ../CAT.c;
perf stat -r 1 -e L1-dcache-loads,L1-dcache-stores,L1-dcache-load-misses,instructions,branch-misses ./a.out;

cd ../../..;
