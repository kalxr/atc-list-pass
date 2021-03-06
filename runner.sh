cd CAT_lib/misc/sample;

filename="many-conflicts.c"
ex=$(echo "$filename" | cut -f 1 -d '.')

clang -O3 -march=native -fdeclspec $filename ../CAT.c;
./a.out;

### APPLY TRANSFORMATION ###

# clang -O1 -emit-llvm -c -Xclang -disable-llvm-passes A.c -o A.bc
clang -O1 -S -emit-llvm -Xclang -disable-llvm-passes -fdeclspec $filename -o "$ex".ll
# clang -emit-llvm -c $filename
noelle-norm "$ex".ll -o "$ex".ll; 
noelle-load -S -load ~/CAT/lib/CAT.so -CAT "$ex".ll -o "$ex".ll; 

# opt -O3 "$ex".ll -o "$ex".ll;
clang -O1 -S -emit-llvm "$ex".ll -o "$ex".ll;
clang -O1 -c -march=native -fdeclspec "$ex".ll;
clang -g -O1 -march=native -fdeclspec "$ex".o ../CAT.c;
./a.out;

cd ../..;
