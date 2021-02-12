# Makefile
#
# In this program there is 1 main program (main.c) and 4 helper
#programs. graph, procs, parse, main_helper
# Here is a list of the .c and .h files:
# main.c
# parse.c 
# parse.h
# m_struct.h
# procs.c
# procs.h
# graphs.c
# graphs.h
# main_helper.h
# main_helper.c
#
# Here is the dependency tree:
#
#
#
#
#                                  -- parse.h -- parse.c
#                               /		   ____graph.h -- graph.c
# prodcom -- -- (main.c) -- m_struct.h --<=
#                               \		   ----procs.h -- procs.c
#                                -- main_helper.h-- action.c
#                                                                                                       
#

CC      = gcc
CFLAGS = -Wall -pedantic -g
OBJECTS = main.o parse.o main_helper.o procs.o
#saurabh
#SCAN_BUILD_DIR = scan-build-out
   		   
main: $(OBJECTS)
	$(CC) $(CFLAGS) -o main $(OBJECTS)
		
main.o: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h
	$(CC) $(CFLAGS) -c main.c

parse.o: parse.c parse.h m_struct.h 
	$(CC) $(CFLAGS) -c parse.c

main_helper.o: main_helper.c main_helper.h 
	$(CC) $(CFLAGS) -c main_helper.c

procs.o: procs.c procs.h main_helper.c main_helper.h
	$(CC) $(CFLAGS) -c procs.c
clean:
	rm -f $(OBJECTS) main
