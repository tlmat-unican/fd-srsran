#!/bin/bash
# This script creates the header and source files (pb.h & pb.cc) in this local folder

read -p "Select the .proto to be compiled: " file

SRC_DIR="."

protoc -I=$SRC_DIR --cpp_out=$SRC_DIR   $file
