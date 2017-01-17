### Introduction

This projects contains mutliple examples to show how to start creating
an OpenWRT application/package.

Feel free to submit new examples and fix errors! :-)

myapp1:
* dummy daemon written C
* the following files will be installed:
  * /usr/bin/myapp1
  * /etc/config/myapp1
  * /etc/init.d/myapp1
* cmake build system
* source code is part of the package

myapp2
* dummy program written in C that just prints out a message
* the following files will be installed:
  * /usr/bin/myapp2
* make build system
* source code is part of the package
* package let you select features called foo and bar

These examples are licensed under CC0-1.0 / placed into Public Domain.

### Build Images and Packages

These are the instructions to build an image
for your router including the example applications:

```
git clone -b lede-17.01 git://git.lede-project.org/source.git
cd source

./scripts/feeds update -a
./scripts/feeds install -a

git clone https://github.com/mwarning/openwrt-example-program.git
cp -rf openwrt-example-program/myapp* package/
rm -rf openwrt-example-program/

make defconfig
make menuconfig
```

Now select the right Target System and Target Profile.
Also select the examples you like to build:

* "Utilities" => "myapp1"
* "Net" => "VPN" => "myapp2"

The package are selected when there is a <*> in front of the name (hit the sapce bar twice).

Finally - build the image:
```
make
```

You can now flash your router using the correct image file inside bin/targets.
The *.ipk packages are located in bin/packages and can be installed e.g. using `opkg install myapp-0.1-1.ipk`.

### Test programs

To test your program you need to login into your router (telnet or ssh).

* myapp1:
You can execute `myapp1` or `/etc/init.d/myapp1 start` on the console.
The application will show a short message before it disconnects
from the console and will run for 60 seconds as background process.

* myapp2
Or just call `myapp2` to start that program.
