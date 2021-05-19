# Performance 1

lame
 4,044,212,975      L1-dcache-loads:u                                             ( +-  0.02% )  (60.00%)
 2,043,397,802      L1-dcache-stores:u                                            ( +-  0.03% )  (40.00%)
   500,774,345      L1-dcache-load-misses:u   #   12.38% of all L1-dcache hits    ( +-  0.02% )  (40.00%)
12,263,164,879      instructions:u                                                ( +-  0.00% )  (60.00%)
         4,506      branch-misses:u                                               ( +- 45.78% )  (60.00%)

            3.0124 +- 0.0268 seconds time elapsed  ( +-  0.89% )

cool
1,046,792,648      L1-dcache-loads:u                                             ( +-  0.21% )  (59.87%)
   44,650,790      L1-dcache-stores:u                                            ( +-  0.68% )  (40.04%)
  125,457,600      L1-dcache-load-misses:u   #   11.98% of all L1-dcache hits    ( +-  0.29% )  (40.13%)
1,895,058,661      instructions:u                                                ( +-  0.21% )  (60.10%)
        1,620      branch-misses:u                                               ( +- 10.10% )  (59.96%)

           0.44488 +- 0.00300 seconds time elapsed  ( +-  0.67% )

 3.8x less L1-dcache-loads
45x   less L1-dcache-stores
 4.0x less L1-dcache-load-misses
 6.5x less instructions
 2.8x less branch-misses

 6.8x speed up

# Performance 2

lame
 4,001,236,688      L1-dcache-loads:u                                             ( +-  0.02% )  (60.00%)
 2,000,138,592      L1-dcache-stores:u                                            ( +-  0.05% )  (40.02%)
   500,164,730      L1-dcache-load-misses:u   #   12.50% of all L1-dcache hits    ( +-  0.04% )  (40.00%)
12,000,856,669      instructions:u                                                ( +-  0.02% )  (60.00%)
       101,794      branch-misses:u                                               ( +-  1.16% )  (59.98%)

            2.6073 +- 0.0906 seconds time elapsed  ( +-  3.47% )

cool
1,001,540,257      L1-dcache-loads:u                                             ( +-  0.08% )  (59.85%)
    1,155,014      L1-dcache-stores:u                                            ( +-  0.12% )  (40.05%)
  124,950,702      L1-dcache-load-misses:u   #   12.48% of all L1-dcache hits    ( +-  0.15% )  (40.15%)
1,627,919,650      instructions:u                                                ( +-  0.06% )  (60.16%)
      100,404      branch-misses:u                                               ( +-  0.08% )  (59.95%)

           0.34239 +- 0.00326 seconds time elapsed  ( +-  0.95% )

   4x  less L1-dcache-loads
1731x  less L1-dcache-stores
   4x  less L1-dcache-load-misses
  12x  less instructions
  same branch misses

   7.6x speed up


