###############################################################################
# M5P01 (RMP) User Manual (Chinese) -- build rules
###############################################################################

LATEXMK ?= latexmk
DOC      = rmp-manual
OUTDIR   = ../..
TARGET   = $(OUTDIR)/CN_M5P01_R6T1_User_Manual.pdf
SOURCES  = $(DOC).tex rmp-style.sty $(wildcard Chapter/*.tex) \
           $(wildcard Figure/*) rmp.bib

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(LATEXMK) -xelatex -interaction=nonstopmode -halt-on-error \
		-outdir=Build $(DOC).tex
	cp Build/$(DOC).pdf $(TARGET)

clean:
	-$(LATEXMK) -C -outdir=Build $(DOC).tex
	-rm -rf Build
	-rm -f $(TARGET)
