################################################################
# For building: ALL_T
# make version:	GNU Make 3.82
# g++ version:	gcc version 4.5.1
# gcc version:	g++ version 4.5.1
################################################################

#### === CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT === ####

# ENTRANCE
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
LIBS		= -l$(SYSLIBS) $(MYLIBS)
RM			= rm -f

# system options
SYSCFLAGS	=
SYSLDFLAGS	=
SYSLIBS		= mysqlclient

# user options
MYCFLAGS	=
MYLDFLAGS	=
MYLIBS		=

# == END SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE == #

# all target
ALL_C_T		= conn_sql getip

ALL_CXX_T	=

ALL_T		= $(ALL_C_T) $(ALL_CXX_T)

# all object
ALL_C_OBJ	= conn_sql.o getip.o

ALL_CXX_OBJ	=

ALL_OBJ		= $(ALL_C_OBJ) $(ALL_CXX_OBJ)

# Targets start here
default:	$(ENTRANCE)

all:	$(ALL_T)

# link object
$(ALL_C_T): % : %.o
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(LOAPATH) $(LIBS) -o $@
$(ALL_CXX_T): % : %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LOAPATH) $(LIBS) -o $@

# dependency
conn_sql.o: conn_sql.c

none:
	@echo "Please do 'make OPTIONS' wehre OPTIONS is one of these:"
	@echo "	$(ALL_T) all"

.PHONY: clean default none all
clean:
	$(RM) $(ALL_OBJ) $(ALL_T)
