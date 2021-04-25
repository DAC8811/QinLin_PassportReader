#用于编译工程的脚本
#需要放置在PassportReader/build目录下执行
#LDFLAGS="/home/ash1/lab_pro/QinLin_cross_build/QiLin_ARM_aarch64/install/lib" \
#LDLIBS="/home/ash1/lab_pro/QinLin_cross_build/QiLin_ARM_aarch64/install/lib" \
cmake .. -DCMAKE_TOOLCHAIN_FILE=/home/gwi/桌面/passport/QiLin_WI_A/toolchain_qilin.cmake \
-DCMAKE_INSTALL_PREFIX:PATH=/home/gwi/桌面/passport/QiLin_WI_A/install/ \
-DCMAKE_BUILD_TYPE=RELEASE \
-DCMAKE_C_FLAGS=" -O3 -DNDEBUG -ffast-math -ftree-vectorize -fno-lto -fPIC -fdata-sections -ffunction-sections -Wl,--gc-sections -fvisibility=hidden " \
-DCMAKE_CXX_FLAGS="-O3 -DNDEBUG -ftree-vectorize -fno-lto -std=gnu++11 -fexceptions -fPIC -fdata-sections -ffunction-sections -Wl,--gc-sections -fvisibility=hidden "
make -j8 & make install
