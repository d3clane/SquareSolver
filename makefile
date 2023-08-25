CXX = g++-13
CXXFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
		   -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
		   -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
		   -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
		   -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
		   -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
		   -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods \
		   -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand \
		   -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix \
		   -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs \
		   -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow \
		   -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie \
		   -fPIE -Werror=vla

PROGRAMDIR = inputFiles
TARGET = quadratic_Solver
OBJECTDIR = Objects
# HEADERS = $(wildcard *.h)
filesCpp = $(wildcard *.cpp)
objects = $(filesCpp:%.cpp=$(OBJECTDIR)/%.o)

.PHONY: all
all: $(PROGRAMDIR)/$(TARGET) 

$(PROGRAMDIR)/$(TARGET): $(objects) 
	$(CXX) $^ -o $(PROGRAMDIR)/$(TARGET) $(CXXFLAGS)

$(OBJECTDIR)/%.o : %.cpp Errors.h # как поменять только файлы используюищие Errors.h при изменении Errors.h
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

.PHONY: clean
clean:
	rm -rf $(OBJECTDIR)/*.o


