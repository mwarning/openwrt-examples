#
# This software is licensed under the Public Domain.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=myapp1
PKG_VERSION:=0.1
PKG_RELEASE:=1

# see https://spdx.org/licenses/
PKG_LICENSE:=CC0-1.0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/myapp1
	SECTION:=utils
	#DEFAULT:=y
	CATEGORY:=Utilities
	TITLE:=Some different dummy application.
	MAINTAINER:= Foo Bar <foo@example.com>
	URL:=https://www.example.com
endef

define Package/myapp1/description
	This is some dummy application.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/myapp1/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/myapp1 $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) files/myapp1.init $(1)/etc/init.d/myapp1
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) files/myapp1.config $(1)/etc/config/myapp1
endef

$(eval $(call BuildPackage,myapp1))
