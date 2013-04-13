0. Introduction:

This project is an example to show how to start creating
an OpenWRT application/package.

For this example, the source code is located within the package
so there is no need to have the code downloaded by the package
Makefile.

This example includes a dummy daemon program written in C,
an UCI configuration file (on the router it will be in )
and a very basic init script (/etc/init.d/myapp).

The following files will be present on the router:

/usr/bin/myapp
/etc/config/myapp
/etc/init.d/myapp

This software is licensed under the Public Domain.

1. Build the image:

These are the instructions to build an image
for your router including myapp:

svn co svn://svn.openwrt.org/openwrt/trunk openwrt
cd openwrt

./scripts/feeds update -a
./scripts/feeds install -a

make defconfig
make menuconfig

# Now select the right Target System and Target Profile
# and myapp in section "Utils".
# Finally - build the image:

make

# Your can now flash your router using the right image in bin/.

2. Test program.

To test your program you need to login into your router (telnet or ssh).
Your can execute "myapp" or "/etc/init.d/myapp start" on the console.

The application will show a short message before it disconnects
from the console and will run for 60 seconds as background process.
