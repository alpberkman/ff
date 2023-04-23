#!/bin/sh

cc w2stdin.c
cat | ./a.out foo bar
