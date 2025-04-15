#!/bin/bash
rm -rf process_states.o
gcc process_states_linux.c -o process_states.o

./process_states.o