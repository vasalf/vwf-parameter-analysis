TARGET = vWFnew

CXX = g++
CXXFLAGS = -std=c++11 -O2
#OPTS = -I /home/kmm/local/include
#BOOST_LIBS = -lboost_mpi -lboost_serialization

RM = rm -rf 

DEPS = hydro.h params.h random_force.h spheres interaction.h vect.h force.h vwf_model.h 
SRC = main.cpp hydro.cpp params.cpp random_force.cpp spheres_interaction.cpp vect.cpp vwf_model.cpp 

OBJS = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

%.o: %.cpp  $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-$(RM) $(TARGET) $(OBJS)
