
SRC_DIR="."

for file in "$SRC_DIR"/*.proto
do
  protoc -I=$SRC_DIR --cpp_out=$SRC_DIR   $file
done
