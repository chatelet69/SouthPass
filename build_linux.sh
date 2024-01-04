mkdir -p linux-build && cd linux-build && cmake -G "Unix Makefiles" ..
make clean
make
cp app ../app
mkdir -p ../linux-output
cp app ../linux-output/app
cp -R style/ ../linux-output/
cp -R components ../linux-output/
echo "Build finished!"