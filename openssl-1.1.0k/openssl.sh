export CC=gcc \
CXX=g++
./Configure --prefix=/home/gwi/桌面/passport/QiLin_ARM_aarch64_IR/install/ linux-aarch64 shared -fPIC -ffast-math -ftree-vectorize -fdata-sections -ffunction-sections -Wl,--gc-sections
make -j8 && make install
