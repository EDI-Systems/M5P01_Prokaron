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
ifeq "$(wildcard nbproject/Makefile-local-PIC32MZ2048_SK.mk)" "nbproject/Makefile-local-PIC32MZ2048_SK.mk"
include nbproject/Makefile-local-PIC32MZ2048_SK.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC32MZ2048_SK
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

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../MProkaron/Benchmark/test.c ../../../MProkaron/Benchmark/Platform/test_PIC32MZ2048EFM100_extra.s ../../../MProkaron/Kernel/kernel.c ../../../MProkaron/Platform/MIPSM/platform_mipsm.c ../../../MProkaron/Platform/MIPSM/platform_mipsm_asm.s

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1372619718/test.o ${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o ${OBJECTDIR}/_ext/878830112/kernel.o ${OBJECTDIR}/_ext/312523307/platform_mipsm.o ${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1372619718/test.o.d ${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o.d ${OBJECTDIR}/_ext/878830112/kernel.o.d ${OBJECTDIR}/_ext/312523307/platform_mipsm.o.d ${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1372619718/test.o ${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o ${OBJECTDIR}/_ext/878830112/kernel.o ${OBJECTDIR}/_ext/312523307/platform_mipsm.o ${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o

# Source Files
SOURCEFILES=../../../MProkaron/Benchmark/test.c ../../../MProkaron/Benchmark/Platform/test_PIC32MZ2048EFM100_extra.s ../../../MProkaron/Kernel/kernel.c ../../../MProkaron/Platform/MIPSM/platform_mipsm.c ../../../MProkaron/Platform/MIPSM/platform_mipsm_asm.s


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
Device=PIC32MZ2048EFM100
ProjectDir="F:\Code_Library\MCU\Mutatus\M5P1_MuProkaron\Project\MPLAB-PIC32MZ2048EFM100\RMP.X"
ConfName=PIC32MZ2048_SK
ImagePath="dist\PIC32MZ2048_SK\${IMAGE_TYPE}\RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="dist\PIC32MZ2048_SK\${IMAGE_TYPE}"
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
	${MAKE}  -f nbproject/Makefile-PIC32MZ2048_SK.mk dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048EFM100
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o: ../../../MProkaron/Benchmark/Platform/test_PIC32MZ2048EFM100_extra.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1850200776" 
	@${RM} ${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -specs=specs.txt -o ${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o ../../../MProkaron/Benchmark/Platform/test_PIC32MZ2048EFM100_extra.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1,--gdwarf-2,-MD="${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o.d"
${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o: ../../../MProkaron/Platform/MIPSM/platform_mipsm_asm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/312523307" 
	@${RM} ${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -specs=specs.txt -o ${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o ../../../MProkaron/Platform/MIPSM/platform_mipsm_asm.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1,--gdwarf-2,-MD="${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o.d"
else
${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o: ../../../MProkaron/Benchmark/Platform/test_PIC32MZ2048EFM100_extra.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1850200776" 
	@${RM} ${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -specs=specs.txt -o ${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o ../../../MProkaron/Benchmark/Platform/test_PIC32MZ2048EFM100_extra.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--gdwarf-2,-MD="${OBJECTDIR}/_ext/1850200776/test_PIC32MZ2048EFM100_extra.o.d"
${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o: ../../../MProkaron/Platform/MIPSM/platform_mipsm_asm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/312523307" 
	@${RM} ${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -specs=specs.txt -o ${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o ../../../MProkaron/Platform/MIPSM/platform_mipsm_asm.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--gdwarf-2,-MD="${OBJECTDIR}/_ext/312523307/platform_mipsm_asm.o.d"
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1372619718/test.o: ../../../MProkaron/Benchmark/test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1372619718" 
	@${RM} ${OBJECTDIR}/_ext/1372619718/test.o.d 
	@${RM} ${OBJECTDIR}/_ext/1372619718/test.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1372619718/test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -I"../../../MProkaron/Include" -Wall -MMD -MF "${OBJECTDIR}/_ext/1372619718/test.o.d" -o ${OBJECTDIR}/_ext/1372619718/test.o ../../../MProkaron/Benchmark/test.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2
	
${OBJECTDIR}/_ext/878830112/kernel.o: ../../../MProkaron/Kernel/kernel.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/878830112" 
	@${RM} ${OBJECTDIR}/_ext/878830112/kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/878830112/kernel.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/878830112/kernel.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -I"../../../MProkaron/Include" -Wall -MMD -MF "${OBJECTDIR}/_ext/878830112/kernel.o.d" -o ${OBJECTDIR}/_ext/878830112/kernel.o ../../../MProkaron/Kernel/kernel.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2
	
${OBJECTDIR}/_ext/312523307/platform_mipsm.o: ../../../MProkaron/Platform/MIPSM/platform_mipsm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/312523307" 
	@${RM} ${OBJECTDIR}/_ext/312523307/platform_mipsm.o.d 
	@${RM} ${OBJECTDIR}/_ext/312523307/platform_mipsm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/312523307/platform_mipsm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -I"../../../MProkaron/Include" -Wall -MMD -MF "${OBJECTDIR}/_ext/312523307/platform_mipsm.o.d" -o ${OBJECTDIR}/_ext/312523307/platform_mipsm.o ../../../MProkaron/Platform/MIPSM/platform_mipsm.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2
	
else
${OBJECTDIR}/_ext/1372619718/test.o: ../../../MProkaron/Benchmark/test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1372619718" 
	@${RM} ${OBJECTDIR}/_ext/1372619718/test.o.d 
	@${RM} ${OBJECTDIR}/_ext/1372619718/test.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1372619718/test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -I"../../../MProkaron/Include" -Wall -MMD -MF "${OBJECTDIR}/_ext/1372619718/test.o.d" -o ${OBJECTDIR}/_ext/1372619718/test.o ../../../MProkaron/Benchmark/test.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2
	
${OBJECTDIR}/_ext/878830112/kernel.o: ../../../MProkaron/Kernel/kernel.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/878830112" 
	@${RM} ${OBJECTDIR}/_ext/878830112/kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/878830112/kernel.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/878830112/kernel.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -I"../../../MProkaron/Include" -Wall -MMD -MF "${OBJECTDIR}/_ext/878830112/kernel.o.d" -o ${OBJECTDIR}/_ext/878830112/kernel.o ../../../MProkaron/Kernel/kernel.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2
	
${OBJECTDIR}/_ext/312523307/platform_mipsm.o: ../../../MProkaron/Platform/MIPSM/platform_mipsm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/312523307" 
	@${RM} ${OBJECTDIR}/_ext/312523307/platform_mipsm.o.d 
	@${RM} ${OBJECTDIR}/_ext/312523307/platform_mipsm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/312523307/platform_mipsm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -I"../../../MProkaron/Include" -Wall -MMD -MF "${OBJECTDIR}/_ext/312523307/platform_mipsm.o.d" -o ${OBJECTDIR}/_ext/312523307/platform_mipsm.o ../../../MProkaron/Platform/MIPSM/platform_mipsm.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -DPKOBSKDEPlatformTool=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)  -no-legacy-libc  -specs=specs.txt $(COMPARISON_BUILD)   -mreserve=data@0x0:0x37F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=PKOBSKDEPlatformTool=1,--defsym=_min_heap_size=0,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)  -no-legacy-libc  -specs=specs.txt $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=0,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/RMP.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
	${RM} -r build/PIC32MZ2048_SK
	${RM} -r dist/PIC32MZ2048_SK

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
