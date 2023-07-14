#!/bin/bash

for f in `find ./ -maxdepth 1 -name "*.cpp" -o -name "*.h" -o -name "*.hpp"`; do 
	echo -e "========== START $f =========="; 
	iwyu $f -Iclang_includes
	echo -e "========== END $f =============\n\n\n"; 
done
