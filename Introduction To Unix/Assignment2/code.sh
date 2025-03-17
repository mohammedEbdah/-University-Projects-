#!/bin/bash


if [ $# -gt 1 ]; then
    echo "There are more than one argument. Just add one."
elif [ $# -eq 0 ]; then
    echo "Add one argument."
else
    while true; do
        echo "Please select a choice:"
        echo "1- List the names of all files that end with .sh only in the given directory."
        echo "2- List the names of all files that are writable in the given directory."
        echo "3- List the name and size of the file with the largest size in the given directory."
        echo "4- List all filenames that have at least one digit in the given directory."
        echo "5- Exit"
        echo "Enter your choice [1-5]"
        read c
        case $c in
            1)
                find $1 -type f -name '*.sh'
                ;;
            2)
                find $1 -type f -perm -u+w
                ;;
            3)
                find $1 -type f -exec ls -s {} + | sort -n -r | head -1
                ;;
            4)
                find $1 -type f -name '*[0-9]*'
                ;;
            5)
                break
                ;;
            *)
                echo "Wrong choice, choose from 1 to 5"
                ;;
        esac
    done
fi
