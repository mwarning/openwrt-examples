#
# This software is licensed under the Public Domain.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=myapp2
PKG_VERSION:=0.1
PKG_RELEASE:=1

# see https://spdx.org/licenses/
PKG_LICENSE:=CC0-1.0

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/myapp2
	SECTION:=net
	# Should this package be selected by default?
	#DEFAULT:=y
	CATEGORY:=Network
	TITLE:=Some other dummy application.
	# Feature FOO also needs libsodium:
	DEPENDS:=+librt +CONFIG_MYAPP2_ENABLE_FOO:libsodium
	MAINTAINER:= Foo Bar <foo@example.com>
	URL:=https://www.example.com
	SUBMENU:=VPN
endef

define Package/myapp2/description
	Some example Programm called myapp2
endef

define Package/myapp2/config
	source "$(SOURCE)/Config.in"
endef

ifeq ($(CONFIG_MYAPP2_ENABLE_FOO),y)
MAKE_FLAGS += -DFOO
endif

ifeq ($(CONFIG_MYAPP2_ENABLE_BAR),y)
MAKE_FLAGS += -DBAR
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
# Nothing to do here for us.
# By default myapp2/src/Makefile will be used.
endef

define Build/Compile
	CFLAGS="$(TARGET_CFLAGS)" CPPFLAGS="$(TARGET_CPPFLAGS)" $(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)
endef

define Package/myapp2/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/myapp2 $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,myapp2))
