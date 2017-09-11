#!/bin/sh

execname="$1"

dirs=`echo $PATH | tr ":" " "`
			        
for d in $dirs
do
	if [ -x "$d/$execname" ]
	then
		echo "$d/$execname"
	fi
done

# Note that we use double quotes to surround $1 because
# it may contain special characters such as 
# *, ?, or spaces, and we don't want these to be 
# interpreted by the shell.
# For the same reason, we continue to surround
# $execname with double quotes.
