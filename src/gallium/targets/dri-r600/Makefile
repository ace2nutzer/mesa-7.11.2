TOP = ../../../..
include $(TOP)/configs/current

LIBNAME = r600_dri.so

PIPE_DRIVERS = \
	$(TOP)/src/gallium/drivers/r600/libr600.a \
	$(TOP)/src/gallium/state_trackers/dri/drm/libdridrm.a \
	$(TOP)/src/gallium/winsys/r600/drm/libr600winsys.a \
	$(TOP)/src/gallium/drivers/trace/libtrace.a \
	$(TOP)/src/gallium/drivers/rbug/librbug.a \
	$(TOP)/src/gallium/drivers/noop/libnoop.a

C_SOURCES = \
	target.c \
	$(COMMON_GALLIUM_SOURCES) \
	$(DRIVER_SOURCES)

DRIVER_DEFINES = \
	-DGALLIUM_RBUG -DGALLIUM_TRACE -DGALLIUM_NOOP

include ../Makefile.dri

DRI_LIB_DEPS +=

symlinks:
