#!/bin/bash
make
touch hello_os
mkdir hello_os_dir
cp os_hello hello_os_dir/
mv hello_os_dir/os_hello hello_os_dir/hello_os
cp os_hello hello_os_dir/
make clean
touch hello_os.txt
grep -n -i os_hello hello_os.c  > hello_os.txt
