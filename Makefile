# Compiler options
CXXFLAGS+=-std=c++11

# objects
obj = hash.o byte.o sha2.o

# Set make paths
BASE=$(shell pwd)
SRCDIR=${BASE}/src
BINDIR=${BASE}/bin
OBJDIR=${BASE}/obj
LIBDIR=${BASE}/lib

# Default installation directory
DESTDIR=/usr/local

all: info mkdir $(obj:.o=)
	@echo "Built ${@}"

$(obj:.o=): $(obj)
	${CXX} ${CXXFLAGS} ${OBJDIR}/${@}.o -o ${BINDIR}/${@}

$(obj):
	${CXX} -c ${CXXFLAGS} ${SRCDIR}/$(@:.o=.cpp) -o ${OBJDIR}/${@}

info:
	@echo "Configuration"
	@echo "DESTDIR=${DESTDIR}"
	@echo

mkdir:
	@mkdir -p ${BINDIR}
	@mkdir -p ${OBJDIR}
	@mkdir -p ${LIBDIR}

clean:
	@rm -vfr ${BINDIR} ${OBJDIR} ${LIBDIR}
	@echo "Cleaned all"

