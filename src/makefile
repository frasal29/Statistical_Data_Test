include ../customize.make

RMTAHOME=..


OBJDIR  = $(RMTAHOME)/obj
LIBDIR  = C:/MinGW/msys/1.0/local/lib
INCDIR1 = $(RMTAHOME)/incl
INCDIR2 = C:/MinGW/msys/1.0/local/include
BINDIR = $(RMTAHOME)/bin

all: check_home compile


banner:
	@echo ========= ENVIRONMENT ==========
	@echo RMTAHOME  = $(RMTAHOME)
	@echo OBJDIR    = $(OBJDIR)
	@echo LIBDIR    = $(LIBDIR)
	@echo INCDIR1   = $(INCDIR1)
	@echo INCDIR2   = $(INCDIR2)
	@echo BINDIR    = $(BINDIR)
	@echo CCOMPILER = $(CCOMPILER)
	@echo LINKLIB   = $(LINKLIB)
	@echo ================================

check_home:
	@echo ""
	@if [ "$(RMTAHOME)" = "" ]; then \
	echo "The variable RMTAHOME is not set.";\
	echo "Call: \"make RMTAHOME=\`pwd\`/..\" or call \"make\" from the rmta home";\
	echo "";\
	exit 1;\
	fi

# -------------------------
# compile all modules 
# -------------------------

compile: main.c execution.c ANOVA_test.c matrix_test.c chi_table_reader.c filter_matrix.c table_reader.c chi_square_test.c test.c
	${CCOMPILER} -c main.c -o $(OBJDIR)/main.o -I $(INCDIR1) -I $(INCDIR2)
	${CCOMPILER} -c execution.c -o $(OBJDIR)/execution.o -I $(INCDIR1) -I $(INCDIR2)
	${CCOMPILER} -c ANOVA_test.c -o $(OBJDIR)/ANOVA_test.o -I $(INCDIR1) -I $(INCDIR2) 
	${CCOMPILER} -c matrix_test.c -o $(OBJDIR)/matrix_test.o -I $(INCDIR1) -I $(INCDIR2)
	${CCOMPILER} -c chi_table_reader.c -o $(OBJDIR)/chi_table_reader.o -I $(INCDIR1) -I $(INCDIR2)
	${CCOMPILER} -c filter_matrix.c -o $(OBJDIR)/filter_matrix.o -I $(INCDIR1) -I $(INCDIR2)
	${CCOMPILER} -c table_reader.c -o $(OBJDIR)/table_reader.o -I $(INCDIR1) -I $(INCDIR2)
	${CCOMPILER} -c chi_square_test.c -o $(OBJDIR)/chi_square_test.o -I $(INCDIR1) -I $(INCDIR2)
	${CCOMPILER} -c test.c -o $(OBJDIR)/test.o -I $(INCDIR1) -I $(INCDIR2)
	
	${CCOMPILER} $(OBJDIR)/main.o \
			 $(OBJDIR)/execution.o \
		       $(OBJDIR)/ANOVA_test.o \
			 $(OBJDIR)/matrix_test.o \
			 $(OBJDIR)/chi_table_reader.o \
			 $(OBJDIR)/filter_matrix.o \
			 $(OBJDIR)/table_reader.o \
			 $(OBJDIR)/chi_square_test.o \
			 $(OBJDIR)/test.o \
			 -L $(LIBDIR) $(LINKLIB) -o $(BINDIR)/rome-test-analyzer.exe


