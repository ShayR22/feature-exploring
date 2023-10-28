#!/bin/python3

print("set_target_properties(example_library PROPERTIES")
print("    UNITY_BUILD_MODE GROUP")
print(")")

BUCKET_SIZE = 10

for i in range(BUCKET_SIZE):
    sources_str = ""
    for j in range(BUCKET_SIZE):
        class_id = i * BUCKET_SIZE + j
        sources_str+= f"custom_class_{class_id}.cpp "
    print(f"set_source_files_properties({sources_str}")
    print(f"    PROPERTIES UNITY_GROUP \"bucket{i}\"")
    print(")")

