testing iwyu(include what you use)

first install it by:
sudo apt-get install -y iwyu

when running on a file use:
wyu header1.hpp -Iclang_includes
adding with "-I" clang_includes as the tool uses clang as some headers
are compiler based and not part of libc

or run the bash script to do this


