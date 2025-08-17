#!/bin/bash

# Rebuild the project
make re

# Run the program with Valgrind
valgrind \
  --leak-check=full \
  --show-leak-kinds=all \
  --track-origins=yes \
  -s \
  --log-file=memcheck1.log \
  --suppressions=valgrind.supp \
  ./minishell