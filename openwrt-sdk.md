# OpenWrt SDK

The SDK makes it possible to compile a package without compiling the whole toolchain (gcc cross compiler etc.).
In this example, we use the OpenWrt 18.06.1 release SDK for target ramips-mt7620:

```
wget https://downloads.openwrt.org/releases/18.06.1/targets/ramips/mt7620/openwrt-sdk-18.06.1-ramips-mt7620_gcc-7.3.0_musl.Linux-x86_64.tar.xz
tar -xvf openwrt-sdk-18.06.1-ramips-mt7620_gcc-7.3.0_musl.Linux-x86_64.tar.xz
cd openwrt-sdk-18.06.1-ramips-mt7620_gcc-7.3.0_musl.Linux-x86_64/
LC_ALL=C ./scripts/feeds update
LC_ALL=C ./scripts/feeds install zerotier
make package/zerotier/compile
```

We use `LC_ALL=C` here to prevent local errors like `.find.bin: loadlocale.c:129: _nl_intern_locale_data: Assertion cnt < (sizeof (_nl_value_type_LC_TIME) / sizeof (_nl_value_type_LC_TIME[0]))' failed.`.
