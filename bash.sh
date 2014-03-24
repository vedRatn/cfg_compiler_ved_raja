# !/bin/bash

rm -rf tmp
mkdir tmp

for file in "test_files"/*.c 
do
	file=`echo $file | cut -d '/' -f2`
	make -f Makefile.cfg FILE=$file
done

echo "\nProcessing correct files"
echo "-------------------------\n"

for file in "test_files"/*.ecfg 
do
	echo $file
	f=`echo $file | cut -d '/' -f2`
	f=`echo $f | cut -d '.' -f1`
	file1=${file}
	./cfglp64 $file -symtab -compile -icode 
	cat ${file}.* > out1
	./cfglp64-sirki $file -symtab -compile -icode 
	cat ${file}.* > out2
	diff -b out1 out2 > tmp/$f
done