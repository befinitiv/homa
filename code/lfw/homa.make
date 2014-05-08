# GNU Make project makefile autogenerated by Premake
ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

CC = /usr/bin/arm-none-eabi-gcc
CXX = /usr/bin/arm-none-eabi-g++
AR = /usr/bin/arm-none-eabi-ar

ifndef RESCOMP
  ifdef WINDRES
    RESCOMP = $(WINDRES)
  else
    RESCOMP = windres
  endif
endif

ifeq ($(config),debug)
  OBJDIR     = build/arm/debug/application/obj
  TARGETDIR  = .
  TARGET     = $(TARGETDIR)/homa.elf
  DEFINES   += -DDEBUG -DSTM32F0
  INCLUDES  += -Isrc -Isrc/framework/include -Isrc/target/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -Wall -Wextra -Werror -std=c99 -O2 -g3 -O0 -mthumb -mcpu=cortex-m0 -msoft-float
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -L. -Tsrc/target/stm32f0/stm32.ld -lvector -lm -Wl,-Map=homa.map,--cref -mthumb -mcpu=cortex-m0 -msoft-float -mfix-cortex-m3-ldrd -nostartfiles
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(CC) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(ARCH) $(LIBS) $(LDFLAGS)
  define PREBUILDCMDS
	@echo Running pre-build commands
	@echo "\n\n--- Starting to build: `date` ---\n\n"
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	@echo "\n\n--- Finished build ---\n\n"
  endef
endif

ifeq ($(config),release)
  OBJDIR     = build/arm/release/application/obj
  TARGETDIR  = .
  TARGET     = $(TARGETDIR)/homa.elf
  DEFINES   += -DRELEASE -DSTM32F0
  INCLUDES  += -Isrc -Isrc/framework/include -Isrc/target/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -Wall -Wextra -Werror -std=c99 -O2 -g3 -mthumb -mcpu=cortex-m0 -msoft-float
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -L. -Tsrc/target/stm32f0/stm32.ld -lvector -lm -Wl,-Map=homa.map,--cref -mthumb -mcpu=cortex-m0 -msoft-float -mfix-cortex-m3-ldrd -nostartfiles
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(CC) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(ARCH) $(LIBS) $(LDFLAGS)
  define PREBUILDCMDS
	@echo Running pre-build commands
	@echo "\n\n--- Starting to build: `date` ---\n\n"
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	@echo "\n\n--- Finished build ---\n\n"
  endef
endif

ifeq ($(config),processor1)
  OBJDIR     = obj/Processor1
  TARGETDIR  = .
  TARGET     = $(TARGETDIR)/homa.elf
  DEFINES   += -DSTM32F0
  INCLUDES  += -Isrc -Isrc/framework/include -Isrc/target/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -Wall -Wextra -Werror -std=c99 -O2 -mthumb -mcpu=cortex-m0 -msoft-float
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -L. -Tsrc/target/stm32f0/stm32.ld -lvector -lm -Wl,-Map=homa.map,--cref -mthumb -mcpu=cortex-m0 -msoft-float -mfix-cortex-m3-ldrd -nostartfiles
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(CC) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(ARCH) $(LIBS) $(LDFLAGS)
  define PREBUILDCMDS
	@echo Running pre-build commands
	@echo "\n\n--- Starting to build: `date` ---\n\n"
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	@echo "\n\n--- Finished build ---\n\n"
  endef
endif

ifeq ($(config),processor2)
  OBJDIR     = obj/Processor2
  TARGETDIR  = .
  TARGET     = $(TARGETDIR)/homa.elf
  DEFINES   += -DSTM32F0
  INCLUDES  += -Isrc -Isrc/framework/include -Isrc/target/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -Wall -Wextra -Werror -std=c99 -O2 -mthumb -mcpu=cortex-m0 -msoft-float
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -L. -Tsrc/target/stm32f0/stm32.ld -lvector -lm -Wl,-Map=homa.map,--cref -mthumb -mcpu=cortex-m0 -msoft-float -mfix-cortex-m3-ldrd -nostartfiles
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LIBS      += 
  LDDEPS    += 
  LINKCMD    = $(CC) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(ARCH) $(LIBS) $(LDFLAGS)
  define PREBUILDCMDS
	@echo Running pre-build commands
	@echo "\n\n--- Starting to build: `date` ---\n\n"
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
	@echo Running post-build commands
	@echo "\n\n--- Finished build ---\n\n"
  endef
endif

OBJECTS := \
	$(OBJDIR)/main.o \
	$(OBJDIR)/nrfProtocol.o \
	$(OBJDIR)/buildVersion.o \
	$(OBJDIR)/interfaceStatisticsDebugging.o \
	$(OBJDIR)/test.o \
	$(OBJDIR)/msg_msgPump.o \
	$(OBJDIR)/systemInitializer.o \
	$(OBJDIR)/logging.o \
	$(OBJDIR)/Assert.o \
	$(OBJDIR)/genericFlawLessProtocol.o \
	$(OBJDIR)/genericFlawLessProtocolPacketDispatcher.o \
	$(OBJDIR)/msgProxy.o \
	$(OBJDIR)/gpio.o \
	$(OBJDIR)/gpio_common_all.o \
	$(OBJDIR)/gpio_common_f0234.o \
	$(OBJDIR)/memory.o \
	$(OBJDIR)/nvic.o \
	$(OBJDIR)/system.o \
	$(OBJDIR)/spi.o \
	$(OBJDIR)/nrf.o \
	$(OBJDIR)/pwr.o \
	$(OBJDIR)/gpio_interrupts.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking homa
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning homa
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
ifeq (posix,$(SHELLTYPE))
	-$(SILENT) cp $< $(OBJDIR)
else
	$(SILENT) xcopy /D /Y /Q "$(subst /,\,$<)" "$(subst /,\,$(OBJDIR))" 1>nul
endif
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.gch=%.d) -c "$<"
endif

$(OBJDIR)/main.o: src/main.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/nrfProtocol.o: src/application/nrfProtocol.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/buildVersion.o: src/application/common/buildVersion.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/interfaceStatisticsDebugging.o: src/application/common/interfaceStatisticsDebugging.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/test.o: src/application/common/test.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/msg_msgPump.o: src/framework/flawless/core/msg_msgPump.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/systemInitializer.o: src/framework/flawless/init/systemInitializer.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/logging.o: src/framework/flawless/logging/logging.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/Assert.o: src/framework/flawless/misc/Assert.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/genericFlawLessProtocol.o: src/framework/flawless/protocol/genericFlawLessProtocol.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/genericFlawLessProtocolPacketDispatcher.o: src/framework/flawless/protocol/genericFlawLessProtocolPacketDispatcher.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/msgProxy.o: src/framework/flawless/protocol/msgProxy.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/gpio.o: src/target/stm32f0/gpio.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/gpio_common_all.o: src/target/stm32f0/gpio_common_all.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/gpio_common_f0234.o: src/target/stm32f0/gpio_common_f0234.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/memory.o: src/target/stm32f0/memory.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/nvic.o: src/target/stm32f0/nvic.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/system.o: src/target/stm32f0/system.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/spi.o: src/target/stm32f0/spi.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/nrf.o: src/target/stm32f0/nrf.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/pwr.o: src/target/stm32f0/pwr.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"
$(OBJDIR)/gpio_interrupts.o: src/target/stm32f0/gpio_interrupts.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -MF $(@:%.o=%.d) -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
endif
