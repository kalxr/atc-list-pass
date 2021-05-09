cd CAT_lib/misc;

filename="performance.c"
ex=$(echo "$filename" | cut -f 1 -d '.')

clang -O1 $filename CAT.c;
perf stat ./a.out;

### APPLY TRANSFORMATION ###

# clang -O1 -emit-llvm -c -Xclang -disable-llvm-passes A.c -o A.bc
clang -O1 -S -emit-llvm -Xclang -disable-llvm-passes $filename -o "$ex".ll
# clang -emit-llvm -c $filename
noelle-norm "$ex".ll -o "$ex".ll; 
noelle-load -S -load ~/CAT/lib/CAT.so -CAT "$ex".ll -o "$ex".ll; 

llc -filetype=obj "$ex".ll;
clang -g "$ex".o CAT.c;
perf stat ./a.out;

cd ../..;
