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
${OBJECTDIR}/_ext/1067945026/rmp_kernel.o: ../../../Source/Kernel/rmp_kernel.c  .generated_files/flags/dspic33ep512mu810_pim/4ca5f396007a9f1ecd3a24ea5ccf42fcc7b6604d .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1067945026" 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Kernel/rmp_kernel.c  -o ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o: ../../../Source/Platform/DSPIC/rmp_platform_dspic.c  .generated_files/flags/dspic33ep512mu810_pim/4bd5826d2340689dcd713328e210e913537b8af5 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/100099300" 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Platform/DSPIC/rmp_platform_dspic.c  -o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o: ../../../Source/Test/rmp_benchmark.c  .generated_files/flags/dspic33ep512mu810_pim/432acb18ff0e633bcf46c3b154ca46feb8f0bf1b .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1730986455" 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Test/rmp_benchmark.c  -o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1067945026/rmp_kernel.o: ../../../Source/Kernel/rmp_kernel.c  .generated_files/flags/dspic33ep512mu810_pim/92316ede0a85dc6a9f73e9e02689bc3e34d627a5 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1067945026" 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Kernel/rmp_kernel.c  -o ${OBJECTDIR}/_ext/1067945026/rmp_kernel.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1067945026/rmp_kernel.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o: ../../../Source/Platform/DSPIC/rmp_platform_dspic.c  .generated_files/flags/dspic33ep512mu810_pim/3b306f887a425b01885762a90d207b1e4ef99a3b .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/100099300" 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Platform/DSPIC/rmp_platform_dspic.c  -o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/100099300/rmp_platform_dspic.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o: ../../../Source/Test/rmp_benchmark.c  .generated_files/flags/dspic33ep512mu810_pim/c241bd31a660b7b86e11c5f0560437ddf6b516bd .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1730986455" 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d 
	@${RM} ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/Test/rmp_benchmark.c  -o ${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1730986455/rmp_benchmark.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    $(COMPARISON_BUILD)  -specs=specs.txt -I"../../../Include" -I"." -O3 -I"bsp/exp16/dspic33ep512mu810_pim" -I"system_config/exp16/dspic33ep512mu810_pim" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o: ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s  .generated_files/flags/dspic33ep512mu810_pim/27646de913ee519d8a762606889274dd831143a2 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/100099300" 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s  -o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    -specs=specs.txt -I"../../../Include" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rmp_test_dspic33ep512mu810.o: rmp_test_dspic33ep512mu810.s  .generated_files/flags/dspic33ep512mu810_pim/2d848dd8fc6a393597f5a69d766514757e68bc61 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d 
	@${RM} ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  rmp_test_dspic33ep512mu810.s  -o ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    -specs=specs.txt -I"../../../Include" -I"." -Wa,-MD,"${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o: ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s  .generated_files/flags/dspic33ep512mu810_pim/b0ae077eea890be3f952697ad6f839cfce35e142 .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/100099300" 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../Source/Platform/DSPIC/rmp_platform_dspic_gcc.s  -o ${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    -specs=specs.txt -I"../../../Include" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/100099300/rmp_platform_dspic_gcc.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rmp_test_dspic33ep512mu810.o: rmp_test_dspic33ep512mu810.s  .generated_files/flags/dspic33ep512mu810_pim/1d3814519328aa9c0b302a83f38e269e24c706f .generated_files/flags/dspic33ep512mu810_pim/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d 
	@${RM} ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  rmp_test_dspic33ep512mu810.s  -o ${OBJECTDIR}/rmp_test_dspic33ep512mu810.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_dspic33ep512mu810_pim=$(CND_CONF)    -specs=specs.txt -I"../../../Include" -I"." -Wa,-MD,"${OBJECTDIR}/rmp_test_dspic33ep512mu810.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
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
