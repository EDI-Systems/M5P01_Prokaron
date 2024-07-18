#####################################################################
#																	 
#	Created by u'nSP IDE		13:43:25	07/18/24
#
#####################################################################




APPDIR	= C:\PROGRA~1\Sunplus\UNSPID~1\unSPIDE

OUTDIR	= .\Debug

CC	= $(APPDIR)\gcc

AS	= $(APPDIR)\xasm16

LD	= $(APPDIR)\xlink16

AR	= $(APPDIR)\xLib16

RESC	= $(APPDIR)\resc

RM	= $(APPDIR)\rm -f

INCLUDES	= -I"C:/Code_Library/M5P01_Prokaron/Project/UNSPIDE-SPCE061A" -I"../../Include" -I"../../../../Program Files/Sunplus/unSPIDE184/SPCE061A/include"

BODY	= -body SPCE061A -bfile "C:\Program Files\Sunplus\unSPIDE184\unSPIDE\Body\SPCE061A.cpt"

BINFILE	= "$(OUTDIR)\RMP.S37"

ARYFILE	= "$(OUTDIR)\RMP.ary"

SBMFILE	= "$(OUTDIR)\RMP.sbm"

OPT	= -S -O3 -gstabs

ASFLAGS	= -t2

CASFLAGS	= -t2

CFLAGS	= $(OPT) -B$(APPDIR)\ $(INCLUDES) 

BINTYPE	= -as

LDFLAGS	= 

EXTRAFLAGS	= 


OBJFILES	= \
	"$(OUTDIR)\Resource.obj" \
	"$(OUTDIR)\rmp_kernel.obj" \
	"$(OUTDIR)\rmp_platform_unsp.obj" \
	"$(OUTDIR)\rmp_platform_unsp_gcc.obj" \
	"$(OUTDIR)\rmp_benchmark.obj" \
	"$(OUTDIR)\rmp_test_spce061a_gcc.obj" 

"$(OUTDIR)\Resource.obj": "C:\Code_Library\M5P01_Prokaron\Project\UNSPIDE-SPCE061A\Resource.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -l "$(OUTDIR)\Resource.lst" -o "$(OUTDIR)\Resource.obj" "C:\Code_Library\M5P01_Prokaron\Project\UNSPIDE-SPCE061A\Resource.asm" 

"$(OUTDIR)\rmp_kernel.obj": "C:\Code_Library\M5P01_Prokaron\Source\Kernel\rmp_kernel.c" "rmp_platform.h" "..\..\Include\Kernel/rmp_kernel.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/rmp_kernel.asm" "C:/Code_Library/M5P01_Prokaron/Source/Kernel/rmp_kernel.c" 
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\rmp_kernel.lst" -o "$(OUTDIR)\rmp_kernel.obj" "$(OUTDIR)\rmp_kernel.asm" 

"$(OUTDIR)\rmp_platform_unsp.obj": "C:\Code_Library\M5P01_Prokaron\Source\Platform\UNSP\rmp_platform_unsp.c" "..\..\Include\Platform/UNSP/rmp_platform_unsp.h" "..\..\Include\Kernel/rmp_kernel.h" "rmp_platform_unsp_conf.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/rmp_platform_unsp.asm" "C:/Code_Library/M5P01_Prokaron/Source/Platform/UNSP/rmp_platform_unsp.c" 
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\rmp_platform_unsp.lst" -o "$(OUTDIR)\rmp_platform_unsp.obj" "$(OUTDIR)\rmp_platform_unsp.asm" 

"$(OUTDIR)\rmp_platform_unsp_gcc.obj": "C:\Code_Library\M5P01_Prokaron\Source\Platform\UNSP\rmp_platform_unsp_gcc.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -l "$(OUTDIR)\rmp_platform_unsp_gcc.lst" -o "$(OUTDIR)\rmp_platform_unsp_gcc.obj" "C:\Code_Library\M5P01_Prokaron\Source\Platform\UNSP\rmp_platform_unsp_gcc.asm" 

"$(OUTDIR)\rmp_benchmark.obj": "C:\Code_Library\M5P01_Prokaron\Source\Test\rmp_benchmark.c" "rmp_test.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/rmp_benchmark.asm" "C:/Code_Library/M5P01_Prokaron/Source/Test/rmp_benchmark.c" 
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\rmp_benchmark.lst" -o "$(OUTDIR)\rmp_benchmark.obj" "$(OUTDIR)\rmp_benchmark.asm" 

"$(OUTDIR)\rmp_test_spce061a_gcc.obj": "C:\Code_Library\M5P01_Prokaron\Project\UNSPIDE-SPCE061A\rmp_test_spce061a_gcc.asm" "..\..\Include\Platform\UNSP\rmp_platform_unsp_gcc.inc" 
	$(AS) $(ASFLAGS) $(INCLUDES) -l "$(OUTDIR)\rmp_test_spce061a_gcc.lst" -o "$(OUTDIR)\rmp_test_spce061a_gcc.obj" "C:\Code_Library\M5P01_Prokaron\Project\UNSPIDE-SPCE061A\rmp_test_spce061a_gcc.asm" 


.SUFFIXES : .c .asm .obj .s37 .tsk .res

all :	 "$(OUTDIR)" $(BINFILE)

"$(OUTDIR)" :
	if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

$(BINFILE) : $(OBJFILES) 
	$(LD) $(BINTYPE) $(ARYFILE) $(BINFILE) $(LDFLAGS) $(BODY) $(EXTRAFLAGS)

compile :	 $(OBJFILES)

clean :
	$(RM) "$(OUTDIR)\Resource.obj" 
	$(RM) "$(OUTDIR)\Resource.lst" 
	$(RM) "$(OUTDIR)\rmp_kernel.obj" 
	$(RM) "$(OUTDIR)\rmp_kernel.lst" 
	$(RM) "$(OUTDIR)\rmp_kernel.asm" 
	$(RM) "$(OUTDIR)\rmp_platform_unsp.obj" 
	$(RM) "$(OUTDIR)\rmp_platform_unsp.lst" 
	$(RM) "$(OUTDIR)\rmp_platform_unsp.asm" 
	$(RM) "$(OUTDIR)\rmp_platform_unsp_gcc.obj" 
	$(RM) "$(OUTDIR)\rmp_platform_unsp_gcc.lst" 
	$(RM) "$(OUTDIR)\rmp_benchmark.obj" 
	$(RM) "$(OUTDIR)\rmp_benchmark.lst" 
	$(RM) "$(OUTDIR)\rmp_benchmark.asm" 
	$(RM) "$(OUTDIR)\rmp_test_spce061a_gcc.obj" 
	$(RM) "$(OUTDIR)\rmp_test_spce061a_gcc.lst" 
	$(RM) $(BINFILE) $(SBMFILE) 

.c.asm:
	$(CC) $(CFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

.asm.obj:
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

