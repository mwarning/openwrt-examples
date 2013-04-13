#
# This software is licensed under the Public Domain.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=myapp
PKG_VERSION:=0.1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk


define Package/myapp
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Some dummy application.
endef

define Package/myapp/description
	This is some dummy application.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/myapp/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/myapp $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) files/myapp.init $(1)/etc/init.d/myapp
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) files/myapp.config $(1)/etc/config/myapp
endef

$(eval $(call BuildPackage,myapp))
