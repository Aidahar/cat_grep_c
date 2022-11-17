#!/bin/bash

cd ../../../
docker rmi -f ereva/valgrind/cat
docker build . -t ereva/valgrind/cat -f materials/build/Valgrind/Dockerfile
docker run --rm ereva/valgrind/cat
