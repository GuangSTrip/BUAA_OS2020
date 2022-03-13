#!/bin/bash
#lab0 Exercise 0.1.4
sed -n '8,32,128,512,1024p' $1>$2
