#
# This software is licensed under the Public Domain.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=example3
PKG_VERSION:=0.1
PKG_RELEASE:=1

# see https://spdx.org/licenses/
PKG_LICENSE:=CC0-1.0

include $(INCLUDE_DIR)/package.mk

define Package/example3
	SECTION:=utils
	# Should this package be selected by default?
	#DEFAULT:=y
	CATEGORY:=Utilities
	TITLE:=Some different dummy application.
	MAINTAINER:= Foo Bar <foo@example.com>
	URL:=https://www.example.com
endef

define Package/example3/description
	This is some dummy application.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/example3/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/example3 $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) files/example3.init $(1)/etc/init.d/example3
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) files/example3.config $(1)/etc/config/example3
endef

$(eval $(call BuildPackage,example3))
