include ./customize.make

ifeq ($(OS),Windows_NT)
	RMTAHOME=%cd%
	DELCOMMAND=del 
else
	RMTAHOME=$(PWD)
	DELCOMMAND=rm -f 
endif


OBJDIR  = $(RMTAHOME)/obj
LIBDIR  = C:/MinGW/msys/1.0/local/lib
INCDIR1 = $(RMTAHOME)/incl
INCDIR2 = C:/MinGW/msys/1.0/local/include
BINDIR = $(RMTAHOME)/bin

all: banner compile


banner:
	@echo ========= ENVIRONMENT ==========
	@echo RMTAHOME   = $(RMTAHOME)
	@echo OBJDIR     = $(OBJDIR)
	@echo LIBDIR     = $(LIBDIR)
	@echo INCDIR     = $(INCDIR)
	@echo BINDIR     = $(BINDIR)
	@echo CCOMPILER  = $(CCOMPILER)
	@echo LINKLIB    = $(LINKLIB)
	@echo DELCOMMAND = $(DELCOMMAND)
	@echo ================================


# -------------------------
# compile all modules 
# -------------------------

compile:  
	cd src && $(MAKE) compile


# ---------------------------
# delete all non-source files
# ---------------------------

clean: 
	cd obj && $(DELCOMMAND) *.o
	$(DELCOMMAND) bin/rome-test-analyzer


