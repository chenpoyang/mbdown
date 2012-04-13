################################################################
# For building: ALL_T
# make version:	GNU Make 3.82
# g++ version:	gcc version 4.5.1
# gcc version:	g++ version 4.5.1
################################################################

#### === CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT === ####

########## Compiler, tools and options

CC			= gcc
CXX			= g++
# DEFINES		= -DNO_DEBUG
DEFINES		=
CFLAGS		= -O2 -Wall -g $(DEFINES)
CXXFLAGS	= -O2 -Wall -g $(DEFINES)
INCPATH		= -I.
LDFLAGS		= $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS		= -l$(SYSLIBS) l$(MYLIBS)

AR			= ar rcu
RANLIB		= ranlib
RM			= rm -f

# system options
SYSCFLAGS	=
SYSLDFLAGS	=
SYSLIBS		=

# user options
MYCFLAGS	=
MYLDFLAGS	=
MYLIBS		=

# == END SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE == #

# all target
ALL_C_T		= down

ALL_CXX_T	=

ALL_T		= $(ALL_C_T) $(ALL_CXX_T)

# all object
ALL_C_OBJ	= freedown.o

ALL_CXX_OBJ	=

ALL_OBJ		= $(ALL_C_OBJ) $(ALL_CXX_OBJ)

# Targets start here
all:	$(ALL_T) $(ALL_OBJ)

down: freedown.o
	$(CC) $(CFLAGS) -o $@ $<

# link object
#$(ALL_C_T): % : %.o
#	$(CC) $(CFLAGS) $(LDFLAGS) $< $(LIBS) -o $@
#$(ALL_CXX_T): % : %.o
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

# pseudo targets
.PHONY: clean default none all

none:
	@echo "Please do 'make OPTIONS' wehre OPTIONS is one of these:"
	@echo "	$(ALL_T) all"

clean:
	$(RM) $(ALL_OBJ) $(ALL_T)

# do not delete
freedown.o: freedown.c
