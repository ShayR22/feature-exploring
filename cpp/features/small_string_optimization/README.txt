see small string optimization by messing with the amount
of string generated and the size of the string

big differrence in allocation between
valgrind --tool=memcheck ./a.out 10000 15
valgrind --tool=memcheck ./a.out 10000 16

after 15 characters (or 14) reserve actually allocates.
Note: might require some messing around as this number depends
on the compiler and compiler version or maybe cpp standard. 
