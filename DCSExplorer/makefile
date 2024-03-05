# # the compiler: g++ for C++ program
# CXX = g++ -std=c++17

# # compiler flags:
# # -pendatic 
# # Without this option, certain GNU extensions and traditional C and C++ features are supported as well. 
# # With this option, they are rejected.
# #  -g    adds debugging information to the executable file
# CXXFLAGS  = -g

# # the build target executable:
# TARGET = DCSExplorer ../DCSDecoder/DCSDecoder

# all: $(TARGET)

# $(TARGET): $(TARGET).cpp
# 	$(CXX) $(CXXFLAGS) $(TARGET).cpp -o executable
# #$(CXX) $(CXXFLAGS) $(TARGET).cpp -o $(TARGET) 

# clean:
# 	$(RM) $(TARGET)


#clang++ -c root.cpp
#clang++ -c players.cpp
#clang++ root.o players.o -o executable

#clang++ root.cpp players.cpp -o executable


# the compiler: g++ for C++ program
CXX = g++ 

# compiler flags:
# -pendatic 
# Without this option, certain GNU extensions and traditional C and C++ features are supported as well. 
# With this option, they are rejected.
#  -g    adds debugging information to the executable file
CXXFLAGS = -std=c++17 -g

SRCS = ../Utilities/BuildDate.cpp DCSExplorer.cpp ../DCSDecoder/DCSDecoder.cpp ../DCSDecoder/DCSDecoderNative.cpp ../DCSDecoder/DCSDecoderEmu.cpp ../DCSDecoder/adsp2100/2100dasm.cpp  ../DCSDecoder/adsp2100/adsp2100.cpp ../DCSDecoder/DCSDecoderZipLoader.cpp ../HiResTimerMPC/HiResTimer.cpp ../miniz-3/miniz.c #../miniz/miniz.c ../miniz/miniz_zip.c
OBJS = $(SRCS:.cpp=.o)
EXECUTABLE = DCSExplorerUniversal

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

#-L/usr/local/lib -lminiz

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)

