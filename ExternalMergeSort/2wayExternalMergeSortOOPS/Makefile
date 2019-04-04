CXX = g++
CXXFLAG = -Wall -g
SRCS = DiskFile.cpp ExtMergeSort.cpp Frame.cpp Main.cpp MainMemory.cpp Page.cpp
OBJS = $(SRCS:.c=.o)
MAIN = ExternalMergeSort

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAG) -o $(MAIN) $(OBJS)

.c.o:
	$(CXX) $(CXXFLAG) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)
