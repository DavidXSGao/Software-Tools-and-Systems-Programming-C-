#!/bin/sh

for i in `ls tests/`
do
    mkdir actual/actual_$i
    cp tests/$i actual/actual_$i
done

# Note that if you use `ls tests` instead of `ls tests/` you will
# iterate over files names with a leading tests/
# Below are two options for dealing with this:

# Option 2 use the basename command to strip off the tests/
#for i in `ls tests`
#do
    #i=`basename $i`
    #mkdir actual/actual_$i
    #cp tests/$i actual/actual_$i
#done


#Option 3 changes into the tests directory 
#cd tests 
#for i in * 
#do
    #mkdir ../actual/actual_$i
    #cp $i ../actual/actual_$i
#done
#cd ..

