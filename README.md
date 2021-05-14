# Benchmark Results
1. 1,000,000 nodes, 1,000 iterations
lame - 5.266238153 seconds, 21,586,502 cache misses
cool - 0.398752196 seconds, 24,376 cache misses


2. 10,000 nodes, 100,000 iterations
lame - 4.806765001 seconds, 21,927 cache misses
cool - 0.350731751 seconds, 138 cache misses

3. 1,000,000 nodes, 10 iterations
lame - 0.100455207 seconds, 379,008 cache misses
cool - 0.070346623 seconds, 42,345 cache misses

4. 1,000,000 nodes, 3 iterations (average performance of 100 runs)
lame - 0.058409 +- 0.000318 seconds time elapsed  ( +-  0.54% ) 
78,444 cache-misses ( +-  4.22% )

cool - 0.057893 +- 0.000456 seconds time elapsed  ( +-  0.79% )
30,516 cache-misses ( +-  3.64% )

5. 10,000,000 nodes, one iteration
lame - 0.49512 +- 0.00613 seconds time elapsed  ( +-  1.24% )
424,740 cache-misses ( +-  4.90% )

cool - 0.56428 +- 0.00252 seconds time elapsed  ( +-  0.45% )
356,580 cache-misses ( +-  1.66% )




# LLVM_middleend_template

This is the template to use for assignments of the Code Analysis and Transformation class at Northwestern University.

Website of the class = https://users.cs.northwestern.edu/~simonec/CAT.html

To build: 
  Compile and install your code by invoking `./run_me.sh`
  The script run_me.sh compiles and installs an LLVM-based compiler that includes your CAT in the directory ~/CAT

To run:
  1) Add your compiler cat-c in your PATH (i.e., `export PATH=~/CAT/bin:$PATH`)

  2) Invoke your compiler to compile a C/C++ program. For example:
  ```sh
    $ cat-c program_to_analyse.c -o mybinary
    $ cat-c -O3 program_to_analyse.c -o mybinary
    $ cat-c -O0 program_to_analyse.bc -o mybinary
  ```
