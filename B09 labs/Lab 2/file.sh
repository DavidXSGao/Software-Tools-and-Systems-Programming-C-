#!/bin/sh

for i in "$@"
do
    if [ -e "$i" ]
    then
       echo "$i" is a file or directory
    else
       echo "$i" is not a file or directory
    fi 
done

# Note that we use "$@" (including the double quotes)
# because this is expanded to "$1" "$2" "$3" etc.

# We want each of our command-line arguments to be quoted
# because they may contain special characters such as 
# *, ?, or spaces, and we don't want these to be 
# interpreted by the shell.

# For the same reason, we put $i in quotes.
