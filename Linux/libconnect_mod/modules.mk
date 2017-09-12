mod_libconnect_mod.la: mod_libconnect_mod.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_libconnect_mod.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_libconnect_mod.la
