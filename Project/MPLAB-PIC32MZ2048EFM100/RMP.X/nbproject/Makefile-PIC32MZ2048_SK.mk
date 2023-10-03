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
FINAL_IMAGE=${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../Source/Kernel/rmp_kernel.c ../../../Source/Platform/MIPSM/rmp_platform_mipsm.c ../../../Source/Platform/MIPSM/rmp_platform_mipsm_gcc.s rmp_test_pic32mz2048efm100.s ../../../Source/Test/rmp_benchmark.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1067945026/rmp_kernel.o ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o ${OBJECTDIR}/rmp_test_pic32mz2048efm100.o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o.d ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o.d ${OBJECTDIR}/rmp_test_pic32mz2048efm100.o.d ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1067945026/rmp_kernel.o ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o ${OBJECTDIR}/rmp_test_pic32mz2048efm100.o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o

# Source Files
SOURCEFILES=../../../Source/Kernel/rmp_kernel.c ../../../Source/Platform/MIPSM/rmp_platform_mipsm.c ../../../Source/Platform/MIPSM/rmp_platform_mipsm_gcc.s rmp_test_pic32mz2048efm100.s ../../../Source/Test/rmp_benchmark.c



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
	${MAKE}  -f nbproject/Makefile-PIC32MZ2048_SK.mk ${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048EFM100
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o: ../../../Source/Platform/MIPSM/rmp_platform_mipsm_gcc.s  .generated_files/flags/PIC32MZ2048_SK/69a343a9600c6254015dadd429034f02ab822e49 .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/108113399" 
	@${RM} ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -D__MPLAB_DEBUGGER_PK3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -specs=specs.txt -o ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o ../../../Source/Platform/MIPSM/rmp_platform_mipsm_gcc.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--gdwarf-2,-MD="${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o.d",-I"." -I"../../../Include" -mdfp="${DFP_DIR}"
	
${OBJECTDIR}/rmp_test_pic32mz2048efm100.o: rmp_test_pic32mz2048efm100.s  .generated_files/flags/PIC32MZ2048_SK/493442fa8e5b91d40d320faf6e480411b412c8c5 .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rmp_test_pic32mz2048efm100.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -D__MPLAB_DEBUGGER_PK3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -specs=specs.txt -o ${OBJECTDIR}/rmp_test_pic32mz2048efm100.o rmp_test_pic32mz2048efm100.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--gdwarf-2,-MD="${OBJECTDIR}/rmp_test_pic32mz2048efm100.o.d",-I"." -I"../../../Include" -mdfp="${DFP_DIR}"
	
else
${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o: ../../../Source/Platform/MIPSM/rmp_platform_mipsm_gcc.s  .generated_files/flags/PIC32MZ2048_SK/8d4ac84b0a9aaee3307b51a69bb4c97556a4e8f2 .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/108113399" 
	@${RM} ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -specs=specs.txt -o ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o ../../../Source/Platform/MIPSM/rmp_platform_mipsm_gcc.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--gdwarf-2,-MD="${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm_gcc.o.d",-I"." -I"../../../Include" -mdfp="${DFP_DIR}"
	
${OBJECTDIR}/rmp_test_pic32mz2048efm100.o: rmp_test_pic32mz2048efm100.s  .generated_files/flags/PIC32MZ2048_SK/6eea46761e31d184e3791b54df535fc01f2a6c7 .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rmp_test_pic32mz2048efm100.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -specs=specs.txt -o ${OBJECTDIR}/rmp_test_pic32mz2048efm100.o rmp_test_pic32mz2048efm100.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--gdwarf-2,-MD="${OBJECTDIR}/rmp_test_pic32mz2048efm100.o.d",-I"." -I"../../../Include" -mdfp="${DFP_DIR}"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1067945026/rmp_kernel.o: ../../../Source/Kernel/rmp_kernel.c  .generated_files/flags/PIC32MZ2048_SK/b7e1d862cec8ecefd430054c4b5161b3931583cf .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1067945026" 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"../../../Include" -O3 -fno-common -I"../../../Include" -I"./" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d" -o ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o ../../../Source/Kernel/rmp_kernel.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o: ../../../Source/Platform/MIPSM/rmp_platform_mipsm.c  .generated_files/flags/PIC32MZ2048_SK/4b937e1d8fbd6661ed3dce931e64c70b7a0868af .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/108113399" 
	@${RM} ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o.d 
	@${RM} ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"../../../Include" -O3 -fno-common -I"../../../Include" -I"./" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o.d" -o ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o ../../../Source/Platform/MIPSM/rmp_platform_mipsm.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o: ../../../Source/Test/rmp_benchmark.c  .generated_files/flags/PIC32MZ2048_SK/dd27d680ba551214101c30c0b5caa65882e95764 .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1730986455" 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"../../../Include" -O3 -fno-common -I"../../../Include" -I"./" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d" -o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o ../../../Source/Test/rmp_benchmark.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2 -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1067945026/rmp_kernel.o: ../../../Source/Kernel/rmp_kernel.c  .generated_files/flags/PIC32MZ2048_SK/7565dabc2574b6a623599d55bd87be50329fc960 .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1067945026" 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"../../../Include" -O3 -fno-common -I"../../../Include" -I"./" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d" -o ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o ../../../Source/Kernel/rmp_kernel.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o: ../../../Source/Platform/MIPSM/rmp_platform_mipsm.c  .generated_files/flags/PIC32MZ2048_SK/50ae7082f24911dfb7cb385e798b8144ecec2a32 .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/108113399" 
	@${RM} ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o.d 
	@${RM} ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"../../../Include" -O3 -fno-common -I"../../../Include" -I"./" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o.d" -o ${OBJECTDIR}/_ext/108113399/rmp_platform_mipsm.o ../../../Source/Platform/MIPSM/rmp_platform_mipsm.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o: ../../../Source/Test/rmp_benchmark.c  .generated_files/flags/PIC32MZ2048_SK/e6c4f78e14628eeca4ac671aa0f1cc7312548beb .generated_files/flags/PIC32MZ2048_SK/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1730986455" 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"../../../Include" -O3 -fno-common -I"../../../Include" -I"./" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d" -o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o ../../../Source/Test/rmp_benchmark.c    -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -Wall -Wextra -mno-dsp -mno-dspr2 -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)    -specs=specs.txt $(COMPARISON_BUILD)   -mreserve=data@0x0:0x37F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=0,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/RMP.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_PIC32MZ2048_SK=$(CND_CONF)    -specs=specs.txt $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=0,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/RMP.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
