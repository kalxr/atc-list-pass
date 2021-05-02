cd CAT_lib/misc;

filename="sample.c"
ex=$(echo "$filename" | cut -f 1 -d '.')

clang -S -emit-llvm $filename -o "$ex".ll
# clang -emit-llvm -c $filename
noelle-norm "$ex".ll -o "$ex".ll; 
noelle-load -S -load ~/CAT/lib/CAT.so -CAT "$ex".ll -o "$ex".ll; 

llc -filetype=obj "$ex".ll;
clang "$ex".o CAT.c;
./a.out;

cd ../..;
