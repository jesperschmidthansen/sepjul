

CC          =  gcc
WARN        = -W -Wall -Wextra -Wpedantic
OPTIM       = -Ofast
RANLIB      =  ranlib
AR          =  ar 
COMPLEX     =  -DCOMPLEX
OMP         =  -fopenmp
OBJECTS     =  sepmisc.o seputil.o separray.o sepinit.o sepprfrc.o sepintgr.o \
	       sepret.o sepmol.o sepcoulomb.o sepsampler.o sepomp.o	


# Shared object 
libseptojul.so:src/septojul.c libsep.a
	gcc $(WARN) $(OPTIM) -fopenmp -fPIC -shared *.o src/septojul.c -o libseptojul.so

# Building library
libsep.a: $(OBJECTS)
	$(AR) r libsep.a $(OBJECTS) 
	$(RANLIB) libsep.a

%.o:src/source/%.c 
	$(CC) $(WARN) $(OPTIM) $(COMPLEX) $(OMP) -c $(CFLAGS) -fPIC -Iinclude/ $<


# Compiling tools
_sep_lattice.o: tools/_sep_lattice.c
	$(CC) $(CFLAGS) $(OMP) -c tools/_sep_lattice.c

_sep_sfg.o: tools/_sep_sfg.c
	$(CC) $(CFLAGS) $(OMP) -c tools/_sep_sfg.c

# Cleaning up
clean: 
	rm -f libsep.a *.o
	rm -f source/*~
	rm -f include/*~
	rm -f *~
	rm -f $(TOOLS)
