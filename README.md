### Introduction

This project contains multiple examples to show how to package an application for [OpenWrt](https://openwrt.org).
The examples were tested with the LEDE-17.01 release.

Feel free to submit new examples and fix errors! :-)

## Examples

example1:
* dummy daemon written C that forks into background and exits after some (configurable) time.
* the following files will be installed:
  * /usr/bin/example1
  * /etc/config/example1
  * /etc/init.d/example1
* cmake build system
* source code is part of the package

example2
* dummy program written in C that just prints out a message
* the following files will be installed:
  * /usr/bin/example2
* make build system
* source code is part of the package
* package let you select features called "foo" and "bar"

example3
* like example1, but runs in background by means of [procd](https://wiki.openwrt.org/inbox/procd-init-scripts)
* this is the preferred way to run a daemon on OpenWrt
* the following files will be installed:
  * /usr/bin/example3
* make build system
* source code is part of the package

These examples are licensed under CC0-1.0 / placed into Public Domain.

Official OpenWrt packages can be found here and are a good reference:
* https://github.com/openwrt/packages
* https://github.com/openwrt-routing/packages

### Build Images and Packages

These are the instructions to build an image
for your router including the example applications:

```
git clone https://github.com/openwrt/openwrt
cd openwrt

./scripts/feeds update -a
./scripts/feeds install -a

git clone https://github.com/mwarning/openwrt-examples.git
cp -rf openwrt-examples/example* package/
rm -rf openwrt-examples/

make defconfig
make menuconfig
```

Now select the right "Target System" and "Target Profile" for your target device.
Also select the examples you like to build:

* "Utilities" => "example1"
* "Network" => "VPN" => "example2"

Packages are selected when there is a <\*> in front of the name (hit the space bar twice).

Finally - build the image:
```
make
```

You can now flash your router using the correct image file inside ./bin/targets/. The images usually contain all build packages already.
The single \*.ipk packages are located in ./bin/packages, in case you want to install them on other devices.

To install/update a package, transfer the ipk file to your target device to /tmp/ using scp.
The package can then be installed calling e.g. `opkg install myapp-0.1-1.ipk`.

### Install/Test Packages

To test your program you need to login into your router (telnet or ssh).

* example1:
You can execute `example1` or `/etc/init.d/example1 start` on the console.
The application will show a short message before it disconnects
from the console and runs for some seconds as background process before it exits.
The duration can be configured in /etc/config/example1.

* example2:
`example2`on the console will just start that program.

### Use Remote Source Location

This is the most common package configuration.
The source code is downloaded as a compressed file from somewhere on the Internet.

First part of an example package Makefile:
```
include $(TOPDIR)/rules.mk

PKG_NAME:=netdata
PKG_VERSION:=1.4.0
PKG_RELEASE:=3
PKG_MAINTAINER:=Bob Smith <bob.smith@example.net>
PKG_LICENSE:=GPL-3.0
PKG_LICENSE_FILES:=COPYING

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=http://firehol.org/download/netdata/releases/v$(PKG_VERSION)
PKG_SOURCE_VERSION:=3028b87ee19e8550df6b9decc49733d595e0bd6e
```

The source code can also be checked out from a remote git repository by branch, tag or commit id:

First part of an example package Makefile:
```
include $(TOPDIR)/rules.mk

PKG_NAME:=stoken
PKG_VERSION:=0.8
PKG_REV:=c4d79ffbf5053e44be4b64da22b1b7fb6a51daf2
PKG_RELEASE:=2

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/cernekee/stoken.git

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_REV).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
```

TODO: show what variables are optional

* PKG_NAME: Package name. Use lower case letters.
* PKG_LICENSE: Package license. See the [SPDX license list](https://spdx.org/licenses/) for valid values.
* PKG_VERSION: The same version as program to be packaged. If you do not know how to version projects, have a look at [semantic versioning](https://semver.org/).
* PKG_RELEASE: Revision of the package. Start at 1 and increase when the package changes. reset to 1 if PKG_VERSION changes.
* PKG_REV: Branch name or git commit id (sha1 hash).
* PKG_SOURCE_URL: Path to a download directory or source control repository (e.g. git, svn, ..)
* PKG_MAINTAINER: Name and Email address of the maintainer. See example for prefered format.
* PKG_LICENSE_FILES: A file in the package that has licensing information.
* PKG_BUILD_DIR: Set explicit build directory, e.g. if the extracted directory does not match the PKG_NAME PKG_VERSION scheme: `PKG_BUILD_DIR:=$(BUILD_DIR)/FooBar-$(PKG_VERSION)`

### Use Local Source Location

For development purposes, it is often useful to specify a local repository on the same computer to apply new commits and build new images/packages without much hassle.

1. Create a soft link called git-src in the same folder of your package Makefile. I needs to point to the .git folder of your local repository checkout.
```
ln -s /my/own/project/repo/example3/.git openwrt/package/example3/git-src
```

2. Call `make menuconfig` in your openwrt folder and enable this feature:
```
"Advanced configuration options (for developers)" => "Enable package source tree override"
```

3. In your git repository folder, create new commits and then rebuild your package in the openwrt folder:

```
make package/example3/{clean,compile} V=s
```

4. Your package should now appear in bin/packages/.

Be aware that changes will only be included in the binary when they are part of a commit in the git repository!
