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
ifeq "$(wildcard nbproject/Makefile-local-dspic33ep512mu810_pim.mk)" "nbproject/Makefile-local-dspic33ep512mu810_pim.mk"
include nbproject/Makefile-local-dspic33ep512mu810_pim.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=dspic33ep512mu810_pim
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../MProkaron/Benchmark/test.c ../../../MProkaron/Kernel/kernel.c ../../../MProkaron/Platform/DSPIC/platform_dspic.c ../../../MProkaron/Platform/DSPIC/platform_dspic_asm.s ../../../MProkaron/Benchmark/Platform/test_DSPIC33EP512MU810_extra.s

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1372619718/test.o ${OBJECTDIR}/_ext/878830112/kernel.o ${OBJECTDIR}/_ext/320537406/platform_dspic.o ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1372619718/test.o.d ${OBJECTDIR}/_ext/878830112/kernel.o.d ${OBJECTDIR}/_ext/320537406/platform_dspic.o.d ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o.d ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1372619718/test.o ${OBJECTDIR}/_ext/878830112/kernel.o ${OBJECTDIR}/_ext/320537406/platform_dspic.o ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o

# Source Files
SOURCEFILES=../../../MProkaron/Benchmark/test.c ../../../MProkaron/Kernel/kernel.c ../../../MProkaron/Platform/DSPIC/platform_dspic.c ../../../MProkaron/Platform/DSPIC/platform_dspic_asm.s ../../../MProkaron/Benchmark/Platform/test_DSPIC33EP512MU810_extra.s


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

# The following macros may be used in the pre and post step lines
Device=dsPIC33EP512MU810
ProjectDir="F:\Code_Library\MCU\Mutatus\M5P1_MuProkaron\Project\MPLAB-DSPIC33EP512MU810\RMP.X"
ConfName=dspic33ep512mu810_pim
ImagePath="dist\dspic33ep512mu810_pim\${IMAGE_TYPE}\RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="dist\dspic33ep512mu810_pim\${IMAGE_TYPE}"
ImageName="RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  .pre ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-dspic33ep512mu810_pim.mk dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU810
MP_LINKER_FILE_OPTION=,--script=p33EP512MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1372619718/test.o: ../../../MProkaron/Benchmark/test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1372619718" 
	@${RM} ${OBJECTDIR}/_ext/1372619718/test.o.d 
	@${RM} ${OBJECTDIR}/_ext/1372619718/test.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../MProkaron/Benchmark/test.c  -o ${OBJECTDIR}/_ext/1372619718/test.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1372619718/test.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../MProkaron/Include" -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1372619718/test.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/878830112/kernel.o: ../../../MProkaron/Kernel/kernel.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/878830112" 
	@${RM} ${OBJECTDIR}/_ext/878830112/kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/878830112/kernel.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../MProkaron/Kernel/kernel.c  -o ${OBJECTDIR}/_ext/878830112/kernel.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/878830112/kernel.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../MProkaron/Include" -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/878830112/kernel.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/320537406/platform_dspic.o: ../../../MProkaron/Platform/DSPIC/platform_dspic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/320537406" 
	@${RM} ${OBJECTDIR}/_ext/320537406/platform_dspic.o.d 
	@${RM} ${OBJECTDIR}/_ext/320537406/platform_dspic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../MProkaron/Platform/DSPIC/platform_dspic.c  -o ${OBJECTDIR}/_ext/320537406/platform_dspic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/320537406/platform_dspic.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../MProkaron/Include" -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/320537406/platform_dspic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1372619718/test.o: ../../../MProkaron/Benchmark/test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1372619718" 
	@${RM} ${OBJECTDIR}/_ext/1372619718/test.o.d 
	@${RM} ${OBJECTDIR}/_ext/1372619718/test.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../MProkaron/Benchmark/test.c  -o ${OBJECTDIR}/_ext/1372619718/test.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1372619718/test.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../MProkaron/Include" -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1372619718/test.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/878830112/kernel.o: ../../../MProkaron/Kernel/kernel.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/878830112" 
	@${RM} ${OBJECTDIR}/_ext/878830112/kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/878830112/kernel.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../MProkaron/Kernel/kernel.c  -o ${OBJECTDIR}/_ext/878830112/kernel.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/878830112/kernel.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../MProkaron/Include" -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/878830112/kernel.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/320537406/platform_dspic.o: ../../../MProkaron/Platform/DSPIC/platform_dspic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/320537406" 
	@${RM} ${OBJECTDIR}/_ext/320537406/platform_dspic.o.d 
	@${RM} ${OBJECTDIR}/_ext/320537406/platform_dspic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../MProkaron/Platform/DSPIC/platform_dspic.c  -o ${OBJECTDIR}/_ext/320537406/platform_dspic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/320537406/platform_dspic.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../MProkaron/Include" -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/320537406/platform_dspic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o: ../../../MProkaron/Platform/DSPIC/platform_dspic_asm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/320537406" 
	@${RM} ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o.d 
	@${RM} ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../MProkaron/Platform/DSPIC/platform_dspic_asm.s  -o ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  -specs=specs.txt -I"../../../MProkaron/Include" -Wa,-MD,"${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o: ../../../MProkaron/Benchmark/Platform/test_DSPIC33EP512MU810_extra.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1850200776" 
	@${RM} ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o.d 
	@${RM} ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../MProkaron/Benchmark/Platform/test_DSPIC33EP512MU810_extra.s  -o ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  -specs=specs.txt -I"../../../MProkaron/Include" -Wa,-MD,"${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o: ../../../MProkaron/Platform/DSPIC/platform_dspic_asm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/320537406" 
	@${RM} ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o.d 
	@${RM} ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../MProkaron/Platform/DSPIC/platform_dspic_asm.s  -o ${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  -specs=specs.txt -I"../../../MProkaron/Include" -Wa,-MD,"${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/320537406/platform_dspic_asm.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o: ../../../MProkaron/Benchmark/Platform/test_DSPIC33EP512MU810_extra.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1850200776" 
	@${RM} ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o.d 
	@${RM} ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../MProkaron/Benchmark/Platform/test_DSPIC33EP512MU810_extra.s  -o ${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  -specs=specs.txt -I"../../../MProkaron/Include" -Wa,-MD,"${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1850200776/test_DSPIC33EP512MU810_extra.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../MProkaron/Include"  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../MProkaron/Include" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif

.pre:
	@echo "--------------------------------------"
	@echo "User defined pre-build step: [copy.bat]"
	@copy.bat
	@echo "--------------------------------------"

# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/dspic33ep512mu810_pim
	${RM} -r dist/dspic33ep512mu810_pim

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
