####################################################################
# For building: ALL_T
# make version:	GNU Make 3.82
# g++ version:	gcc version 4.5.1
# gcc version:	g++ version 4.5.1
####################################################################

#### === CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT === ####

ENTRANCE	= none

########## Compiler, tools and options

CC			= gcc
CXX			= g++
# DEFINES		= -DNO_DEBUG
DEFINES		=
CFLAGS		= -O2 -Wall -g $(DEFINES)
CXXFLAGS	= -O2 -Wall -g $(DEFINES)
INCPATH		=
LOAPATH		=
LDFLAGS		= $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS		= $(SYSLIBS) $(MYLIBS)

AR			= ar rcu
RANLIB		= ranlib
RM			= rm -f

# system options
SYSCFLAGS	=
SYSLDFLAGS	=
SYSLIBS		= -lpthread

# user options
MYCFLAGS	=
MYLDFLAGS	=
MYLIBS		=

# == END SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE == #

# all target
ALL_C_T		= mbdown

ALL_CXX_T	=

ALL_T		= $(ALL_C_T) $(ALL_CXX_T)

# all object
ALL_C_OBJ	= global.o mbdown.o url.o synchro.o bmsg.o

ALL_CXX_OBJ	=

ALL_OBJ		= $(ALL_C_OBJ) $(ALL_CXX_OBJ)

# Targets start here
default:	$(ENTRANCE)

all:	$(ALL_T) $(ALL_OBJ)

mbdown: $(ALL_C_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LOAPATH) $(LIBS) -o mbdown $(ALL_C_OBJ)

none:
	@echo "Please do 'make OPTIONS' wehre OPTIONS is one of these:"
	@echo "	$(ALL_T) all"

clean:
	$(RM) $(ALL_OBJ) $(ALL_T)

# list targets that do not create files
.PHONY: clean default none all

# DO NOT DELETE
global.o	: global.c global.h
bmsg.o		: bmsg.c bmsg.h global.h
synchro.o	: synchro.c synchro.h
url.o		: url.c url.h global.h synchro.h
mbdown.o	: mbdown.c global.h
