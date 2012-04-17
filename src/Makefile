####################################################################
# For building: ALL_T
# make version:	GNU Make 3.82
# g++ version:	gcc version 4.5.1
# gcc version:	g++ version 4.5.1
####################################################################

#### === CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT === ####

########## Compiler, tools and options

CC			= gcc
CFLAGS		= -O2 -Wall -g $(DEFINES)
CXXFLAGS	= -O2 -Wall -g $(DEFINES)

# all target
ALL_T		= mbdown

# all object
ALL_OBJ	= global.o mbdown.o url.o synchro.o bmsg.o http.o progress.o

# Targets start here
all:	$(ALL_OBJ) $(ALL_T)

mbdown: $(ALL_OBJ)
	$(CC) $(CFLAGS) -o mbdown -lpthread $(ALL_OBJ)

clean:
	$(RM) $(ALL_OBJ) $(ALL_T)

# list targets that do not create files
.PHONY: clean all

# DO NOT DELETE
global.o	: global.c global.h
bmsg.o		: bmsg.c bmsg.h global.h
synchro.o	: synchro.c synchro.h
url.o		: url.c url.h global.h synchro.h
mbdown.o	: mbdown.c global.h url.h synchro.h progress.h
http.o		: http.c http.h url.h global.h
progress.o	: progress.c progress.h
