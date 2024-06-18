#!/bin/bash

echo 1 || echo 2 && echo 3
echo expected: 1

echo 1 && echo 2 || echo 3
echo expected: 1,2

echo 1 || echo 2 || echo 3
echo expected: 1

echo 1 && echo 2 && echo 3
echo 'expected: 1,2,3'

asdf && echo 2 && echo 3
echo 'expected: command not found'

asdf || echo 2 && echo 3
echo 'expected: command not found, 2 ,3'

asdf && asdf || echo 3
echo 'expected command not found, 3'