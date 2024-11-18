cd build/ &&
rm -rf * &&
cmake .. &&
make &&
printf "\n\n" &&
./test
