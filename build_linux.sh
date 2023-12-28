mkdir -p linux-build
cd linux-build
cmake -G "Unix Makefiles" ..
make
cp app ../app