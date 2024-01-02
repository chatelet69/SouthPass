mkdir -p linux-build
cd linux-build
cmake -G "Unix Makefiles" ..
make
cp app ../app
mkdir -p ../linux-output
cp app ../linux-output/app
cp -R style/ ../linux-output/