# Introduction

This project contains multiple examples to show how to package an application for [OpenWrt](https://openwrt.org).
The examples were tested with the OpenWrt-18.06.0 release.

Feel free to submit new examples and fix errors! :-)

## Examples

### example1

* dummy daemon written in C that forks into background and exits after some (configurable) time.
* the following files will be installed:
  * /usr/bin/example1
  * /etc/config/example1
  * /etc/init.d/example1
* uses cmake build system
* source code is part of the package

### example2

* dummy program written in C that just prints out a message
* the following files will be installed:
  * /usr/bin/example2
* uses make build system
* source code is part of the package
* package let you select features called "foo" and "bar"

### example3

* like example1, but runs in background by means of [procd](https://wiki.openwrt.org/inbox/procd-init-scripts)
* this is the preferred way to run a program as a daemon on OpenWrt
* the following files will be installed:
  * /usr/bin/example3
* make build system
* source code is part of the package

These examples are licensed under CC0-1.0 / placed into Public Domain.

Official OpenWrt packages can be found here and are a good reference:
* https://github.com/openwrt/packages
* https://github.com/openwrt-routing/packages

The official OpenWrt documentation can be found here:
* https://wiki.openwrt.org/doc/techref/initscripts

## Build and Install Images and Packages

These are the instructions to build an image for your router including the example applications:

```
git clone https://github.com/openwrt/openwrt
cd openwrt

# Install packages from feeds
./scripts/feeds update -a
./scripts/feeds install -a

# Install the package manually
git clone https://github.com/mwarning/openwrt-examples.git
cp -rf openwrt-examples/example* package/
rm -rf openwrt-examples/

make menuconfig
```

Now the build menu opens.

### Build menu

The build menu let you select the correct "Target System" and "Target Profile" for your target device.
Also select the examples you like to build:

* "Utilities" => "example1"
* "Network" => "VPN" => "example2"

Use the space bar to change package selections:

* <> Package is not build
* <M> Package is build but not included in the router image
* <\*> Package is build and included in the router image

### Start the build

Finally - build the image:

```
make
```

This might take one to three hours. The sources are downloaded (into `./dl/`), the toolchain is build, then the packages and finally the router images. Subsequent builds will be much faster.

You can now flash your router using the image file inside `./bin/targets/`.
The single \*.ipk packages are located in `./bin/packages`, in case you want to install them afterwards on devices that already run OpenWrt.

To install/update a package, transfer the ipk file to your target device to /tmp/ using `scp`.
The package can then be installed calling e.g. `opkg install /tmp/myapp-0.1-1.ipk`.

Notes:

* Use `make -j4` to speed up compilation using multiple CPU cores.
* `make download` downloads all sources from the Internet into `./dl/`.
* `make package/example3/{clean,compile} V=s` will only build the package example3

### Install image

If the router does not run OpenWrt, then use the correct openwrt-\*-factory.bin image from `./bin/targets`.
Sometimes the router does not accept the image. Reasons might be:
* wrong image file, check the hardware revision of the router vs. the one of the image file (e.g. v1).
** Devices can have different hardware between hardware revisions, even if the model description stays the same.
** For TP-Link devices, the number after the point are software changes only an can be ignored (e.g. v7.1 vs. v7.2).
* The device name is too long. Try to rename the image to something shorter (e.g. `image.bin`).
* The device is vendor locked. The image would need to be signed by the vendor. This is unfortunate. You might need to use a soldering iron to access the console and use tftp to apply the image. Check openwrt.org.

If the router already runs OpenWrt, then use the openwrt-\*-sysupgrade.bin images.
Transfert the image file to `/tmp/` on the device and use sysupgrade to apply the image:

```
sysupgrade /tmp/openwrt-*-sysupgrade.bin
```

Add `-n` to keep configuration files.

## Install/Test Packages

To test your program you need to login in to your router (telnet or ssh).

* example1:
You can execute `example1` or `/etc/init.d/example1 start` on the console.
The application will show a short message before it disconnects from the console and runs for some seconds as background process before it exits.
The duration can be configured in /etc/config/example1.

* example2:
`example2` on the console will just start that program.

## Use Remote Source Location

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
```

This example uses the release source files that are created for each git tag:
```
include $(TOPDIR)/rules.mk

PKG_NAME:=stoken
PKG_VERSION:=0.8
PKG_RELEASE:=2

PKG_SOURCE_URL:=https://codeload.github.com/cernekee/stoken/tar.gz/v$(PKG_VERSION)?
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_HASH:=1b3ccaa01cbb7548ef268d8b562059452826dc774529303c494418d1a450ca97
```
(The '?' at the end of the is a feature of github.com to get a file specified as in PKG_SOURCE)
In the future, PKG_SOURCE_URL_FILE can be used to override PKG_SOURCE to be append PKG_SOURCE_URL


TODO: show what variables are optional

* PKG_NAME: Package name. Use lower case letters.
* PKG_LICENSE: Package license. See the [SPDX license list](https://spdx.org/licenses/) for valid values.
* PKG_VERSION: The same version as program to be packaged. If you do not know how to version projects, have a look at [semantic versioning](https://semver.org/).
* PKG_RELEASE: Revision of the package. Start at 1 and increase when the package changes. reset to 1 if PKG_VERSION changes.
* PKG_REV: Branch name or git commit id (sha1 hash).
* PKG_SOURCE_URL: Path to a download directory or source control repository (e.g. git, svn, ..)
* PKG_SOURCE: Name of the source file to be downloaded. Will be appended to PKG_SOURCE_URL. The file be cached in dl/.
* PKG_SOURCE_URL_FILE: Name of the source file to be downloaded. If set, will be used instead of PKG_SOURCE to be append to PKG_SOURCE_URL. (Not yet implemented)
* PKG_MAINTAINER: Name and Email address of the maintainer. See example for prefered format.
* PKG_LICENSE_FILES: A file in the package that has licensing information.
* PKG_BUILD_DIR: Set explicit build directory, e.g. if the extracted directory does not match the PKG_NAME PKG_VERSION scheme: `PKG_BUILD_DIR:=$(BUILD_DIR)/FooBar-$(PKG_VERSION)`.
* PKG_HASH: sha256 hash of the source package in `./dl/`.

## Use Local Source Location

For development purposes, it is often useful to specify a local (git) repository on the same computer to apply new commits and build new images/packages without much hassle.

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
git commit -m "some new change to test"
```
(Note: The package still needs to be selected in the `make menuconfig` menu)

4. Your package should now appear in bin/packages/.

Be aware that changes will only be included in the binary when they are part of a commit in the git repository!

## Build package for all architectures

Sometimes it is necessary to build packages for all architectures without building the images for all targets.
This script build the package once for each architecture:

```
#!/bin/sh

# dumpinfo.pl is used to get all targets configurations:
# https://git.openwrt.org/?p=buildbot.git;a=blob;f=phase1/dumpinfo.pl

./dumpinfo.pl architectures | while read pkgarch target1 rest; do
  echo "CONFIG_TARGET_${target1%/*}=y" > .config
  echo "CONFIG_TARGET_${target1%/*}_${target1#*/}=y" >> .config
  echo "CONFIG_PACKAGE_example1=y" >> .config

  # Debug output
  echo "pkgarch: $pkgarch, target1: $target1"

  make defconfig
  make -j4 tools/install
  make -j4 toolchain/install

  # Build package
  make package/example1/{clean,compile}

  # Free space (optional)
  rm -rf build_dir/target-*
  rm -rf build_dir/toolchain-*
done
```

## Random notes

* packages build into the image will be enabled by default (like `/etc/init.d/program enable` has been called. Creates a link in `/etc/rc.d/`).
  * You can build an `option enabled 0` in your /etc/config/program configuration file, but it might create confusion 
  * Or place `[ -f /etc/openwrt_release ] || exit 0` as your second line in the package Makefile. This will disable the init script from being called. 
* Files and directories put into ./files will be included into the router image.
  * For example, ./files/etc/example.txt will appear in the image files system as /etc/example.txt.
  * Usually, use a package to install custom files.
* The common way to run scripts only once during first boot is to put them into /etc/uci-defaults/
  * The scripts there will be deleted after first execution.
* If squashfs is used ony the router, then /rom/ will contain the read only base of the file system.
  * the sqashfs file system consists of a compressed base and layers of changes.
  * The `firstboot` command can throw away all layers/changes and the device will be as it was just flashed.
