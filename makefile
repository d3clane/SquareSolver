CXX = g++-13
CXXFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla
TARGET = quadratic_Solver

.PHONY: all
all: $(TARGET)

$(TARGET): Solver.o InputAndOutput.o StringAndCharFuncs.o StringEquationFuncs.o DoubleComparisons.o main.o TestingMode.o Errors.o
	$(CXX) $^ -o $(TARGET) $(CXXFLAGS)

DoubleComparisons.o: DoubleComparisons.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

Solver.o: Solver.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

InputAndOutput.o: InputAndOutput.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

StringAndCharFuncs.o: StringAndCharFuncs.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

StringEquationFuncs.o: StringEquationFuncs.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

TestingMode.o: TestingMode.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

main.o: main.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

Errors.o: Errors.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)


.PHONY: clean
clean:
	rm -rf *.o


