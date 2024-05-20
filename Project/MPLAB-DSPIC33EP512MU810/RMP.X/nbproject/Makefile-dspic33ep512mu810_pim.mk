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
SOURCEFILES_QUOTED_IF_SPACED=../../../Source/Kernel/rmp_kernel.c ../../../Source/Platform/DSPIC/rmp_platform_dspic.c ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s ../../../Source/Test/rmp_benchmark.c rmp_test_dspic33ep512mu810.s

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1067945026/rmp_kernel.o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1067945026/rmp_kernel.o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o

# Source Files
SOURCEFILES=../../../Source/Kernel/rmp_kernel.c ../../../Source/Platform/DSPIC/rmp_platform_dspic.c ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s ../../../Source/Test/rmp_benchmark.c rmp_test_dspic33ep512mu810.s



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
	${MAKE}  -f nbproject/Makefile-dspic33ep512mu810_pim.mk ${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU810
MP_LINKER_FILE_OPTION=,--script=p33EP512MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1067945026/rmp_kernel.o: ../../../Source/Kernel/rmp_kernel.c  .generated_files/flags/dspic33ep512mu810_pim/157bd1918a8b6faa03250cd2d1d94f199ac2423d .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1067945026" 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Kernel/rmp_kernel.c  -o ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o: ../../../Source/Platform/DSPIC/rmp_platform_dspic.c  .generated_files/flags/dspic33ep512mu810_pim/3c5190e527b7d5bc0be660cf824c79709dbc111e .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/100099300" 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Platform/DSPIC/rmp_platform_dspic.c  -o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o: ../../../Source/Test/rmp_benchmark.c  .generated_files/flags/dspic33ep512mu810_pim/3300029863f45388ff96a42e690129f01629bef1 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1730986455" 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Test/rmp_benchmark.c  -o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1067945026/rmp_kernel.o: ../../../Source/Kernel/rmp_kernel.c  .generated_files/flags/dspic33ep512mu810_pim/db18f94d2170ef512ebbead252315880d8d0d009 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1067945026" 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Kernel/rmp_kernel.c  -o ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o: ../../../Source/Platform/DSPIC/rmp_platform_dspic.c  .generated_files/flags/dspic33ep512mu810_pim/2fb6adf6402fcc4809b4d156ab5cd871d7ab5e06 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/100099300" 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Platform/DSPIC/rmp_platform_dspic.c  -o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o: ../../../Source/Test/rmp_benchmark.c  .generated_files/flags/dspic33ep512mu810_pim/fa8b5c3f4fee6dd918612dd622a372f9a74badf9 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1730986455" 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Test/rmp_benchmark.c  -o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o: ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s  .generated_files/flags/dspic33ep512mu810_pim/e45cec1fa636df526ca4dcfeb84d57bf7d32d80b .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/100099300" 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s  -o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    -specs=specs.txt -I"../../../Include" -I"." -I"../../../Include" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rmp_test_dspic33ep512mu810.o: rmp_test_dspic33ep512mu810.s  .generated_files/flags/dspic33ep512mu810_pim/143acadc36651ab923d44736b564578d3472390f .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d 
	@${RM} ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  rmp_test_dspic33ep512mu810.s  -o ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    -specs=specs.txt -I"../../../Include" -I"." -I"../../../Include" -I"." -Wa,-MD,"${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o: ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s  .generated_files/flags/dspic33ep512mu810_pim/b2aa0479e56b56bbb79b3e15b4554288c35447b2 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/100099300" 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s  -o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    -specs=specs.txt -I"../../../Include" -I"." -I"../../../Include" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rmp_test_dspic33ep512mu810.o: rmp_test_dspic33ep512mu810.s  .generated_files/flags/dspic33ep512mu810_pim/d31216cb4027f3772de17afce689663e9b431705 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d 
	@${RM} ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  rmp_test_dspic33ep512mu810.s  -o ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    -specs=specs.txt -I"../../../Include" -I"." -I"../../../Include" -I"." -Wa,-MD,"${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"."  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/RMP.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/RMP.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/RMP.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
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
