CXX=g++
CXXFLAGS=-Wall -Wextra -Wshadow -std=c++11 -O2

SRCS=main.cpp run.cpp params.cpp
OBJS=$(subst cpp,o,$(SRCS)) 
MODEL_OBJS=model/hydro.o model/spheres_interaction.o model/random_force.o model/vect.o model/vwf_model.o model/params.o
HEADERS=$(wildcard *.h)

all: run_genetics

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

run_genetics: $(OBJS)
	$(MAKE) -C ./model
	$(CXX) $(OBJS) $(MODEL_OBJS) -o run_genetics

clean:
	cd model && make clean
	rm -Rf run_genetics $(OBJS)
