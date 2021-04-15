#该脚本需要放置在libusb-1.0.23目录下执行
CFLAGS="-O3 -ffast-math -ftree-vectorize -fno-lto -fPIC -fdata-sections -ffunction-sections -Wl,--gc-sections" \
CXXFLAGS="-O3 -ffast-math -ftree-vectorize -fno-lto -fPIC -fdata-sections -ffunction-sections -Wl,--gc-sections" \
./configure --prefix=/home/gwi/桌面/passport/QiLin_ARM_aarch64/install/ --host=aarch64-linux-gnu --enable-shared=yes --enable-udev=no
make -j8 && make install
