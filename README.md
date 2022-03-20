# Introduction

This project contains multiple examples to show how to package an application for [OpenWrt](https://openwrt.org).
The examples were tested with the OpenWrt 21.02.2 release. Keep in mind that this is more of a personal collection of notes rather than a documentation.

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

## Official Documentation

Official OpenWrt packages can be found here and are a good reference:
* https://github.com/openwrt/packages
* https://github.com/openwrt-routing/packages

Init script documentation can be found here:
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
* "Utilities" => "example3"
* "Network" => "VPN" => "example2"

Use the space bar to change package selections:

* `< >` Package is not build
* `<M>` Package is build but not included in the router image
* `<*>` Package is build and included in the router image

### Start the build

Finally - build the image:

```
make
```

This might take one to three hours. The sources are downloaded (into `./dl/`), the toolchain is build, then the packages and finally the router images. Subsequent builds will be much faster. Just select other packages using `make menuconfig` and call `make` again.

You can now flash your router using the image file inside `./bin/targets/`.
The single \*.ipk packages are located in `./bin/packages`, in case you want to install them afterwards on devices that already run OpenWrt.

To install/update a package, transfer the ipk file to your target device to /tmp/ using `scp`.
The package can then be installed calling e.g. `opkg install /tmp/myapp-0.1-1.ipk`.

Notes:

* Use `` make -j `nproc` `` to speed up compilation using multiple CPU cores.
* `make download` downloads all sources from the Internet into `./dl/`. Useful for offline compilation.
* `make package/example3/{clean,compile} V=s` will only build the package example3.
* Add `V=s` to any make calls for verbose/debug output (e.g. `make V=s`). `V=sc` for even higher verbosity.

### Install Image

If the router does not run OpenWrt, then use the correct openwrt-\*-factory.bin image from `./bin/targets`.
Sometimes the router does not accept the image. Reasons might be:

* wrong image file, check the hardware revision of the router vs. the one of the image file (e.g. v1).
  * Devices can have different hardware between hardware revisions, even if the model description stays the same.
  * For TP-Link devices, the number after the point are software changes only and can be ignored (e.g. v7.1 vs. v7.2).
* The device name is too long. Try to rename the image to something shorter (e.g. `image.bin`).
* The device is vendor locked. The image would need to be signed by the vendor. This is unfortunate. You might need to use a soldering iron to access the console and use tftp to apply the image. Check openwrt.org.

If the router already runs OpenWrt, then use the openwrt-\*-sysupgrade.bin images.
Transfer the image file to `/tmp/` on the device (usually via `scp <image> root@192.168.1.1:/tmp/`) and use sysupgrade to apply the image:

```
sysupgrade /tmp/openwrt-*-sysupgrade.bin
```

Changes in `/etc/config` and files listed `/etc/sysupgrade.conf` will be preserved. But installed packaged are lost and need to be installed again. `sysupgrade -n <image>` will flash an image and does not preseve any configuration.

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
PKG_RELEASE:=$(AUTORELEASE)
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
PKG_RELEASE:=$(AUTORELEASE)

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
PKG_RELEASE:=$(AUTORELEASE)

PKG_SOURCE_URL:=https://codeload.github.com/cernekee/stoken/tar.gz/v$(PKG_VERSION)?
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_HASH:=1b3ccaa01cbb7548ef268d8b562059452826dc774529303c494418d1a450ca97
```
(The '?' at the end of the is a feature of github.com to get a file specified as in PKG_SOURCE)
In the future, PKG_SOURCE_URL_FILE can be used to override PKG_SOURCE to be append PKG_SOURCE_URL


TODO: show what variables are optional

* PKG_NAME: Package name. Use lower case letters.
* PKG_LICENSE: Package license. See the [SPDX license list](https://spdx.org/licenses/) for valid values.
* PKG_LICENSE_FILES: Path to the license file in the source code.
* PKG_VERSION: The same version as program to be packaged. If you do not know how to version projects, have a look at [semantic versioning](https://semver.org/).
* PKG_RELEASE: Revision of the package. Start at 1 and increase when the package changes. Reset to 1 if PKG_VERSION changes. Nowadays you can just use `$(AUTORELEASE)`, which will the task for you.
* PKG_REV: Branch name or git commit id (sha1 hash).
* PKG_SOURCE_URL: Path to a download directory or source control repository (e.g. git, svn, ..)
* PKG_SOURCE: Name of the source file to be downloaded. Will be appended to PKG_SOURCE_URL. The file be cached in dl/.
* PKG_SOURCE_URL_FILE: Name of the source file to be downloaded. If set, will be used instead of PKG_SOURCE to be append to PKG_SOURCE_URL. (Not yet implemented)
* PKG_MAINTAINER: Name and Email address of the maintainer. See example for prefered format.
* PKG_BUILD_DIR: Set explicit build directory, e.g. if the extracted directory does not match the PKG_NAME PKG_VERSION scheme: `PKG_BUILD_DIR:=$(BUILD_DIR)/FooBar-$(PKG_VERSION)`.
* PKG_HASH: sha256 hash of the source package in `./dl/`.

### Specific Build Variables

For projects build with autotools:

```
CONFIGURE_ARGS += --disable-zip
```

This results in a configure call like `./configure --disable-zip`.

```
CONFIGURE_VARS += ac_cv=yes
```
This results in a configure call like `ac_cv=yes ./configure`.

`HOST_CONFIGURE_ARGS`: Similar to `CONFIGURE_ARGS`, but used when build for the host system architecture. This is needed for building the build tools.

## Path Variables

`$(STAGING_DIR)` refers to the build root. E.g. use `$(STAGING_DIR)/usr/lib` to reference the `usr/lib/` folder that contains the libraries build for the target architecture.

`$(PKG_BUILD_DIR)` refers to the extraced source code package or repository checkout.

## Use Local Source Location

For development purposes, it is often useful to specify a local (git) repository on the same computer to apply new commits and build new images/packages without much hassle.

1. Create a soft link called git-src in the same folder of your package Makefile. I needs to point to the .git folder of your local repository checkout.
```
ln -s /my/own/project/repo/example3/.git feeds/packages/example3/git-src
```

2. Call `make menuconfig` in your openwrt folder and enable this feature:
```
"Advanced configuration options (for developers)" => "Enable package source tree override"
```

3. In your git repository folder, create new commits:

```
git commit -m "some new change to test"
```
(Note: The package still needs to be selected in the `make menuconfig` menu)

4. Rebuild your package in the openwrt folder

```
make package/example3/{clean,compile}
```

5. Your package should now appear in bin/packages/.

Be aware that changes will only be included in the binary when they are part of a commit in the git repository!

## Build package for all architectures

Sometimes it is necessary to build packages for all architectures without building the images for all targets.
This script build the package once for each architecture:

```
#!/bin/sh

# dumpinfo.pl is used to get all targets configurations:
# https://git.openwrt.org/?p=buildbot.git;a=blob;f=scripts/dumpinfo.pl

./dumpinfo.pl architectures | while read pkgarch target1 rest; do
  echo "CONFIG_TARGET_${target1%/*}=y" > .config
  echo "CONFIG_TARGET_${target1%/*}_${target1#*/}=y" >> .config
  echo "CONFIG_PACKAGE_example1=y" >> .config

  # Debug output
  echo "pkgarch: $pkgarch, target1: $target1"

  make defconfig
  make -j `nproc` tools/install
  make -j `nproc` toolchain/install

  # Build package
  make package/example1/{clean,compile}

  # Free space (optional)
  rm -rf build_dir/target-*
  rm -rf build_dir/toolchain-*
done
```

## "Dirty patches detected, please refresh and review the diff"

When you update a package, but do not update the patches (e.g. `openwrt/feeds/packages/net/netcat/patches/`), then the patches there might still apply, but you might encounter warnings about offsets/hunks. This is not a big problem, but the OpenWrt pipeline checks might fail because of `dirty patches` and you need to refresh the patches:

```
make package/netcat/{clean,refresh}
```

Commit the changed patches and you should be fine.

## Inspect IPK Packages

Sometimes it is useful to look inside an `*.ipk` package. Let's try to inspect the contents of `htop_2.2.0-2_mips_24kc.ipk`:

```
$ tar -xvf htop_2.2.0-2_mips_24kc.ipk
./debian-binary
./data.tar.gz
./control.tar.g
```

```
$ cat debian-binary
2.0
```
Contains the version of the packaging standard. You see the Debian heritage. :P

```
$ tar -xvf control.tar.gz
./
./control
./postinst
./prerm
```

These files are all plain text files.

* `control` contains the package name, version, dependencies names, maintainer information and other information.
* `postinst` is a shell script. It is executed after a packages is installed.
* `prerm` is also a shell script. It is executed before an installed package is removed.

```
$ tar -xvf data.tar.gz
./
./usr/
./usr/bin/
./usr/bin/htop
```

Notes:
 * the `$` signifies the command prompt of a shell/terminal.

## Using the SDK

See [here](openwrt-sdk.md).

## Using the Image Builder

See [here](https://openwrt.org/docs/guide-user/additional-software/imagebuilder)

## Preserve Configuration Files On Update

Files in `/etc/config` are preserved by default when a package is updated, others are not. To extend that to files in other places, they must be listed in the package Makefile:

```
define Package/example1/conffiles
/etc/example1.conf
endef
```
(Note that there must no be a whitespace in front of the file path)

## Random notes

* packages build into the image will be enabled by default (like `/etc/init.d/program enable` has been called. Creates a link in `/etc/rc.d/`).
  * You can add a setting like `option enabled 0` in your /etc/config/program configuration file, but it might create confusion when `/etc/init.d/program [enable|disable]` is used.
  * Or place `[ -f /etc/openwrt_release ] || exit 0` as your second line in the package Makefile. This will disable the init script from being enabled.
* Files and directories put into ./files will be included into the router image.
  * For example, ./files/etc/example.txt will appear in the image files system as `/etc/example.txt`.
  * existing files will be overwritten,
* The common way to run scripts only once during first boot is to put them into `/etc/uci-defaults/`
  * The scripts there will be deleted after first execution.
* If the squashfs file system is used on the router, then the read only base of the file system is accesssible via `root`.
  * The sqashfs file system consists of a compressed base and layers of changes.
  * The `firstboot` command can throw away all layers/changes and the device will be as it was just flashed. Do not forget to reboot.
* `/etc/config/system` and `/etc/config/network` can be recreated calling `/bin/config_generate` after being deleted. For restoring a deleted `/etc/config/wireless`, use `wifi config` command to recreate it.
* For installing files, `$(CP) ./files/* $(1)/` in the package Makefiles install section is a nice shortcut to install a bunch of files into the image root file system. Otherwise multiple lines would need to be used. This is the preferred way.
* if no hotplug event is received in `/etc/hotplug.d/iface/` (e.g `eth0`), then make a dummy section in `/etc/config/network` to trigger hotplug events on that interface:  
```
  config eth0
    option ifname 'eth0'
    option proto 'none'
```
* "*** Warning - bad CRC, using default environment" from the bootloader often only means that there is no configuration stored and the defaults are used. Save the settings to get rid of the message.
* The DOWNLOAD_MIRROR environment variable can contain `;` separated paths/locations source package files to fetch for `dl/`. The CONFIG_LOCALMIRROR variable in `make menuconfig` serves the same purpose.
