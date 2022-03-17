#!/bin/bash
make
touch hello_os
mkdir hello_os_dir
cp os_hello hello_os_dir/
mv os_hello hello_os
cp hello_os hello_os_dir/
mv hello_os os_hello
make clean
touch hello_os.txt
grep -n -i os_hello hello_os.c  > hello_os.txt
