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

for file in "test_files"/*.cfg 
do
	echo $file
	f=`echo $file | cut -d '/' -f2`
	f=`echo $f | cut -d '.' -f1`
	file1=${file}
	./cfglp $file -compile -icode -symtab -ast -tokens -lra
	cat ${file}.* > out1
	./cfglp64-sirki $file -compile -icode -symtab -ast -tokens -lra
	cat ${file}.* > out2
	diff -b -B out1 out2 > tmp/$f
done