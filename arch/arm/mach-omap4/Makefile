#
# Makefile for the linux kernel.
#

# Common support
obj-y := irq.o id.o io.o sdrc.o control.o mux.o prcm.o\
		devices.o serial.o gpmc.o timer-gp.o \

obj-$(CONFIG_OMAP_MCBSP) += mcbsp.o

# Functions loaded to SRAM
obj-$(CONFIG_ARCH_OMAP3)		+= sram34xx.o

# SMS/SDRC
# obj-$(CONFIG_ARCH_OMAP3)		+= sdrc3xxx.o

# Power Management
ifeq ($(CONFIG_PM),y)
obj-y					+= pm.o
obj-$(CONFIG_PM_DEBUG)			+= pm-debug.o
endif

# SmartReflex driver
obj-$(CONFIG_OMAP_SMARTREFLEX)		+= smartreflex.o

# Clock framework
#obj-$(CONFIG_ARCH_OMAP3)		+= clock34xx.o
ifeq ($(CONFIG_OMAP3_PM),)
       obj-y += clock.o powerdomain.o clockdomain.o
       # Clock framework
       obj-$(CONFIG_ARCH_OMAP3) += clock34xx.o pm34xx.o sleep34xx.o
       # Power Management
       obj-$(CONFIG_PM) += pm.o
else
       obj-y += prcm_34xx.o pm_34xx.o clock_34xx.o sram-fn_34xx.o resource_34xx.o constraint_34xx.o pm_idle_34xx.o sleep_34xx.o
       # Power Management
endif

# Specific board support
obj-$(CONFIG_MACH_OMAP_GENERIC)		+= board-generic.o
obj-$(CONFIG_MACH_OMAP_3430SDP)		+= board-3430sdp.o \
					   hsmmc.o \
					   usb-musb.o \
					   usb-ehci.o \
					   board-3430sdp-flash.o
obj-$(CONFIG_MACH_OMAP_4430VIRTIO)      += board-4430sdp.o \
                                           board-4430sdp-flash.o
# TUSB 6010 chips
obj-$(CONFIG_MACH_OMAP2_TUSB6010)	+= usb-tusb6010.o

