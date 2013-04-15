### Introduction:

This project is an example to show how to start creating
an OpenWRT application/package.

For this example, the source code is located within the package.<br />
This way there is no need to have the code being downloaded by the package
Makefile.

This example includes a dummy daemon program written in C,<br />
an UCI configuration file and a very basic init script.
CMake is used for building the source code.

The following files will be present on the router:

* /usr/bin/myapp
* /etc/config/myapp
* /etc/init.d/myapp

This software is licensed under the Public Domain.<br />
It was tested using OpenWRT "Attitude Adjustment".

### Build the image:

These are the instructions to build an image
for your router including myapp:

<pre>
svn co svn://svn.openwrt.org/openwrt/trunk openwrt
cd openwrt

./scripts/feeds update -a
./scripts/feeds install -a

git clone https://github.com/mwarning/openwrt-example-program.git
cp -rf openwrt-example-program/myapp/ package/
rm -rf openwrt-example-program/

make defconfig
make menuconfig
</pre>

Now select the right Target System and Target Profile.<br />
Also mark myapp in section "Utilities" twice until it is marked as <*>.<br />
Finally - build the image:
<pre>
make
</pre>

You can now flash your router using the right image file in bin/.

### Test program.

To test your program you need to login into your router (telnet or ssh).<br />
You can execute `myapp` or `/etc/init.d/myapp start` on the console.

The application will show a short message before it disconnects<br />
from the console and will run for 60 seconds as background process.
