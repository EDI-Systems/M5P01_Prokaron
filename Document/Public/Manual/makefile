###############################################################################
# M5P01 (RMP) User Manual -- build rules
#
# Requirements: a TeX Live installation with XeLaTeX, latexmk, the packages
# used by rmp-style.sty, and the fonts in ./Font.
#
#   make          build Build/rmp-manual.pdf and copy it to
#                 ../../EN_M5P01_R6T1_User_Manual.pdf
#   make clean    remove build artefacts
###############################################################################

LATEXMK ?= latexmk
DOC      = rmp-manual
OUTDIR   = ../..
TARGET   = $(OUTDIR)/EN_M5P01_R6T1_User_Manual.pdf
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
