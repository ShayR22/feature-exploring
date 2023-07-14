#!/bin/bash

OUTPUT_FILE="iwyu-output-file"

if [[ -f "$OUTPUT_FILE" ]]; then
	rm "$OUTPUT_FILE"
fi


for f in `find ./ -maxdepth 1 -name "*.cpp" -o -name "*.h" -o -name "*.hpp"`; do
	echo -e "========== START $f ==========";
	iwyu $f -Iclang_includes 2>&1 | tee -a $OUTPUT_FILE
	echo -e "========== END $f =============\n\n\n";
done

echo -e "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
grep -rn $OUTPUT_FILE -e 'should remove these lines:\|- #include'
rm $OUTPUT_FILE
