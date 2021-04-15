./autogen.sh
./configure --host=aarch64-linux-gnu CC=gcc CXX=g++ --prefix=/home/gwi/桌面/passport/QiLin_ARM_aarch64_IR/install/ CPPFLAGS="-fPIC"
make -j8 && make install
