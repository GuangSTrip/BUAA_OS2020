#!bin/bash
#lab0-Extra
touch warning.txt
touch result.txt
#gcc $1 -o test
gcc -Wall $1 -o test 2> warning.txt
a=$?
grep  warning warning.txt > result.txt
sed -i 's/warning: //g' result.txt
if [ $a -eq 0 ]
then 
i=1
touch stdin.txt
while [ $i -le $2 ]
do
echo $i>stdin.txt
./test < stdin.txt >> result.txt
i=$[$i+1]
done
rm stdin.txt
fi
pwd >> result.txt
