#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/src/pin_manager.c mcc_generated_files/src/cpuint.c mcc_generated_files/src/wdt.c mcc_generated_files/src/protected_io.S mcc_generated_files/src/clkctrl.c mcc_generated_files/src/system.c mcc_generated_files/src/slpctrl.c mcc_generated_files/src/bod.c mcc_generated_files/mcc.c mcc_generated_files/driver_isr.c mcc_generated_files/device_config.c font.c lcd.c light_ws2812.c main.c applications/snake.c utilities.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/src/pin_manager.o ${OBJECTDIR}/mcc_generated_files/src/cpuint.o ${OBJECTDIR}/mcc_generated_files/src/wdt.o ${OBJECTDIR}/mcc_generated_files/src/protected_io.o ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o ${OBJECTDIR}/mcc_generated_files/src/system.o ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o ${OBJECTDIR}/mcc_generated_files/src/bod.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/driver_isr.o ${OBJECTDIR}/mcc_generated_files/device_config.o ${OBJECTDIR}/font.o ${OBJECTDIR}/lcd.o ${OBJECTDIR}/light_ws2812.o ${OBJECTDIR}/main.o ${OBJECTDIR}/applications/snake.o ${OBJECTDIR}/utilities.o
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/src/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/src/cpuint.o.d ${OBJECTDIR}/mcc_generated_files/src/wdt.o.d ${OBJECTDIR}/mcc_generated_files/src/protected_io.o.d ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o.d ${OBJECTDIR}/mcc_generated_files/src/system.o.d ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o.d ${OBJECTDIR}/mcc_generated_files/src/bod.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/driver_isr.o.d ${OBJECTDIR}/mcc_generated_files/device_config.o.d ${OBJECTDIR}/font.o.d ${OBJECTDIR}/lcd.o.d ${OBJECTDIR}/light_ws2812.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/applications/snake.o.d ${OBJECTDIR}/utilities.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/src/pin_manager.o ${OBJECTDIR}/mcc_generated_files/src/cpuint.o ${OBJECTDIR}/mcc_generated_files/src/wdt.o ${OBJECTDIR}/mcc_generated_files/src/protected_io.o ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o ${OBJECTDIR}/mcc_generated_files/src/system.o ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o ${OBJECTDIR}/mcc_generated_files/src/bod.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/driver_isr.o ${OBJECTDIR}/mcc_generated_files/device_config.o ${OBJECTDIR}/font.o ${OBJECTDIR}/lcd.o ${OBJECTDIR}/light_ws2812.o ${OBJECTDIR}/main.o ${OBJECTDIR}/applications/snake.o ${OBJECTDIR}/utilities.o

# Source Files
SOURCEFILES=mcc_generated_files/src/pin_manager.c mcc_generated_files/src/cpuint.c mcc_generated_files/src/wdt.c mcc_generated_files/src/protected_io.S mcc_generated_files/src/clkctrl.c mcc_generated_files/src/system.c mcc_generated_files/src/slpctrl.c mcc_generated_files/src/bod.c mcc_generated_files/mcc.c mcc_generated_files/driver_isr.c mcc_generated_files/device_config.c font.c lcd.c light_ws2812.c main.c applications/snake.c utilities.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATtiny1614
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/src/protected_io.o: mcc_generated_files/src/protected_io.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/protected_io.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/protected_io.o 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/protected_io.o.ok ${OBJECTDIR}/mcc_generated_files/src/protected_io.o.err 
	 ${MP_CC} $(MP_EXTRA_AS_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -DDEBUG  -x assembler-with-cpp -c -D__$(MP_PROCESSOR_OPTION)__  -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/protected_io.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/protected_io.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/protected_io.o -o ${OBJECTDIR}/mcc_generated_files/src/protected_io.o mcc_generated_files/src/protected_io.S  -DXPRJ_default=$(CND_CONF)  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/mcc_generated_files/src/protected_io.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1
	
else
${OBJECTDIR}/mcc_generated_files/src/protected_io.o: mcc_generated_files/src/protected_io.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/protected_io.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/protected_io.o 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/protected_io.o.ok ${OBJECTDIR}/mcc_generated_files/src/protected_io.o.err 
	 ${MP_CC} $(MP_EXTRA_AS_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x assembler-with-cpp -c -D__$(MP_PROCESSOR_OPTION)__  -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/protected_io.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/protected_io.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/protected_io.o -o ${OBJECTDIR}/mcc_generated_files/src/protected_io.o mcc_generated_files/src/protected_io.S  -DXPRJ_default=$(CND_CONF)  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/mcc_generated_files/src/protected_io.o.asm.d"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/src/pin_manager.o: mcc_generated_files/src/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/pin_manager.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/pin_manager.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/pin_manager.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/pin_manager.o  -o ${OBJECTDIR}/mcc_generated_files/src/pin_manager.o mcc_generated_files/src/pin_manager.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/cpuint.o: mcc_generated_files/src/cpuint.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/cpuint.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/cpuint.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/cpuint.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/cpuint.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/cpuint.o  -o ${OBJECTDIR}/mcc_generated_files/src/cpuint.o mcc_generated_files/src/cpuint.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/wdt.o: mcc_generated_files/src/wdt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/wdt.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/wdt.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/wdt.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/wdt.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/wdt.o  -o ${OBJECTDIR}/mcc_generated_files/src/wdt.o mcc_generated_files/src/wdt.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/clkctrl.o: mcc_generated_files/src/clkctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/clkctrl.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/clkctrl.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o  -o ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o mcc_generated_files/src/clkctrl.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/system.o: mcc_generated_files/src/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/system.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/system.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/system.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/system.o  -o ${OBJECTDIR}/mcc_generated_files/src/system.o mcc_generated_files/src/system.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/slpctrl.o: mcc_generated_files/src/slpctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/slpctrl.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/slpctrl.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o  -o ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o mcc_generated_files/src/slpctrl.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/bod.o: mcc_generated_files/src/bod.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/bod.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/bod.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/bod.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/bod.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/bod.o  -o ${OBJECTDIR}/mcc_generated_files/src/bod.o mcc_generated_files/src/bod.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d" -MT "${OBJECTDIR}/mcc_generated_files/mcc.o.d" -MT ${OBJECTDIR}/mcc_generated_files/mcc.o  -o ${OBJECTDIR}/mcc_generated_files/mcc.o mcc_generated_files/mcc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/driver_isr.o: mcc_generated_files/driver_isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver_isr.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver_isr.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/driver_isr.o.d" -MT "${OBJECTDIR}/mcc_generated_files/driver_isr.o.d" -MT ${OBJECTDIR}/mcc_generated_files/driver_isr.o  -o ${OBJECTDIR}/mcc_generated_files/driver_isr.o mcc_generated_files/driver_isr.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/device_config.o: mcc_generated_files/device_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/device_config.o.d" -MT "${OBJECTDIR}/mcc_generated_files/device_config.o.d" -MT ${OBJECTDIR}/mcc_generated_files/device_config.o  -o ${OBJECTDIR}/mcc_generated_files/device_config.o mcc_generated_files/device_config.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/font.o.d" -MT "${OBJECTDIR}/font.o.d" -MT ${OBJECTDIR}/font.o  -o ${OBJECTDIR}/font.o font.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/lcd.o: lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd.o.d 
	@${RM} ${OBJECTDIR}/lcd.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/lcd.o.d" -MT "${OBJECTDIR}/lcd.o.d" -MT ${OBJECTDIR}/lcd.o  -o ${OBJECTDIR}/lcd.o lcd.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/light_ws2812.o: light_ws2812.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/light_ws2812.o.d 
	@${RM} ${OBJECTDIR}/light_ws2812.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/light_ws2812.o.d" -MT "${OBJECTDIR}/light_ws2812.o.d" -MT ${OBJECTDIR}/light_ws2812.o  -o ${OBJECTDIR}/light_ws2812.o light_ws2812.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o  -o ${OBJECTDIR}/main.o main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/applications/snake.o: applications/snake.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/applications" 
	@${RM} ${OBJECTDIR}/applications/snake.o.d 
	@${RM} ${OBJECTDIR}/applications/snake.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/applications/snake.o.d" -MT "${OBJECTDIR}/applications/snake.o.d" -MT ${OBJECTDIR}/applications/snake.o  -o ${OBJECTDIR}/applications/snake.o applications/snake.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/utilities.o: utilities.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/utilities.o.d 
	@${RM} ${OBJECTDIR}/utilities.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/utilities.o.d" -MT "${OBJECTDIR}/utilities.o.d" -MT ${OBJECTDIR}/utilities.o  -o ${OBJECTDIR}/utilities.o utilities.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/mcc_generated_files/src/pin_manager.o: mcc_generated_files/src/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/pin_manager.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/pin_manager.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/pin_manager.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/pin_manager.o  -o ${OBJECTDIR}/mcc_generated_files/src/pin_manager.o mcc_generated_files/src/pin_manager.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/cpuint.o: mcc_generated_files/src/cpuint.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/cpuint.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/cpuint.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/cpuint.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/cpuint.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/cpuint.o  -o ${OBJECTDIR}/mcc_generated_files/src/cpuint.o mcc_generated_files/src/cpuint.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/wdt.o: mcc_generated_files/src/wdt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/wdt.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/wdt.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/wdt.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/wdt.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/wdt.o  -o ${OBJECTDIR}/mcc_generated_files/src/wdt.o mcc_generated_files/src/wdt.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/clkctrl.o: mcc_generated_files/src/clkctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/clkctrl.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/clkctrl.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o  -o ${OBJECTDIR}/mcc_generated_files/src/clkctrl.o mcc_generated_files/src/clkctrl.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/system.o: mcc_generated_files/src/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/system.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/system.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/system.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/system.o  -o ${OBJECTDIR}/mcc_generated_files/src/system.o mcc_generated_files/src/system.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/slpctrl.o: mcc_generated_files/src/slpctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/slpctrl.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/slpctrl.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o  -o ${OBJECTDIR}/mcc_generated_files/src/slpctrl.o mcc_generated_files/src/slpctrl.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/src/bod.o: mcc_generated_files/src/bod.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/bod.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/src/bod.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/src/bod.o.d" -MT "${OBJECTDIR}/mcc_generated_files/src/bod.o.d" -MT ${OBJECTDIR}/mcc_generated_files/src/bod.o  -o ${OBJECTDIR}/mcc_generated_files/src/bod.o mcc_generated_files/src/bod.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d" -MT "${OBJECTDIR}/mcc_generated_files/mcc.o.d" -MT ${OBJECTDIR}/mcc_generated_files/mcc.o  -o ${OBJECTDIR}/mcc_generated_files/mcc.o mcc_generated_files/mcc.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/driver_isr.o: mcc_generated_files/driver_isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver_isr.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver_isr.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/driver_isr.o.d" -MT "${OBJECTDIR}/mcc_generated_files/driver_isr.o.d" -MT ${OBJECTDIR}/mcc_generated_files/driver_isr.o  -o ${OBJECTDIR}/mcc_generated_files/driver_isr.o mcc_generated_files/driver_isr.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/mcc_generated_files/device_config.o: mcc_generated_files/device_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/device_config.o.d" -MT "${OBJECTDIR}/mcc_generated_files/device_config.o.d" -MT ${OBJECTDIR}/mcc_generated_files/device_config.o  -o ${OBJECTDIR}/mcc_generated_files/device_config.o mcc_generated_files/device_config.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/font.o.d" -MT "${OBJECTDIR}/font.o.d" -MT ${OBJECTDIR}/font.o  -o ${OBJECTDIR}/font.o font.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/lcd.o: lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd.o.d 
	@${RM} ${OBJECTDIR}/lcd.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/lcd.o.d" -MT "${OBJECTDIR}/lcd.o.d" -MT ${OBJECTDIR}/lcd.o  -o ${OBJECTDIR}/lcd.o lcd.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/light_ws2812.o: light_ws2812.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/light_ws2812.o.d 
	@${RM} ${OBJECTDIR}/light_ws2812.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/light_ws2812.o.d" -MT "${OBJECTDIR}/light_ws2812.o.d" -MT ${OBJECTDIR}/light_ws2812.o  -o ${OBJECTDIR}/light_ws2812.o light_ws2812.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o  -o ${OBJECTDIR}/main.o main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/applications/snake.o: applications/snake.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/applications" 
	@${RM} ${OBJECTDIR}/applications/snake.o.d 
	@${RM} ${OBJECTDIR}/applications/snake.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/applications/snake.o.d" -MT "${OBJECTDIR}/applications/snake.o.d" -MT ${OBJECTDIR}/applications/snake.o  -o ${OBJECTDIR}/applications/snake.o applications/snake.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/utilities.o: utilities.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/utilities.o.d 
	@${RM} ${OBJECTDIR}/utilities.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=attiny1614  -I "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\include"  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -MD -MP -MF "${OBJECTDIR}/utilities.o.d" -MT "${OBJECTDIR}/utilities.o.d" -MT ${OBJECTDIR}/utilities.o  -o ${OBJECTDIR}/utilities.o utilities.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=attiny1614  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"   -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="dist\${CND_CONF}\${IMAGE_TYPE}\code.${IMAGE_TYPE}.map"    -o dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	
	
	
	
	
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=attiny1614  -B "E:\Program Files (x86)\Microchip\MPLABX\v5.10\packs\Atmel\ATtiny_DFP\1.3.238\gcc\dev\attiny1614"  -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="dist\${CND_CONF}\${IMAGE_TYPE}\code.${IMAGE_TYPE}.map"    -o dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}\\avr-objcopy -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/code.${IMAGE_TYPE}.hex"
	
	
	
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
