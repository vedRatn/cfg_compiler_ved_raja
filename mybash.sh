# !/bin/bash

make -f Makefile.cfglp
./cfglp test_files/$1.cs306.cfg -d -eval > out1
./cfglp64-sirki test_files/$1.cs306.cfg -d -eval > out2
diff -y out1 out2