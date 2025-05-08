CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -std=c++20
LDFLAGS := -lm
COVFLAGS :=
GCOV_FLAGS :=
GTEST_FLAGS := --gtest_color=yes
RUN_FLAGS :=

SOURCESDIR := ./lib
TARGETDIR := .
TESTSDIR := ./tests
BUILDDIR := ./build
GCOVDIR := ./gcov
LCOVDIR := ./lcov

SUBDIRS := . list vector array
FULLSOURCEDIRS :=
$(foreach dir,$(SUBDIRS),$(eval FULLSOURCEDIRS += $(TESTSDIR)/$(dir)))

TESTSOURCES :=
$(foreach dir,$(FULLSOURCEDIRS),$(eval TESTSOURCES += $(wildcard $(dir)/*.cc)))

# TESTSOURCES += $(wildcard $(TESTSDIR)/*.cc)
TESTOBJECTS := $(patsubst %.cc,%.o,$(foreach file,$(TESTSOURCES),$(file)))
TESTGCNO := $(patsubst %.cc,%.gcno,$(foreach file,$(TESTSOURCES),$(file)))

all: test

# Check and create directories
.PHONY: check_dirs
check_dirs:
	@mkdir -p $(BUILDDIR)

rebuild: clean all

#---------------------------------------------------------#

test: CXXFLAGS += -DTESTS
test: LDFLAGS += -lgtest -lgtest_main -pthread
test: $(TESTOBJECTS)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $(GCOV_FLAGS) $(filter %.o,$(foreach object,$^,$(BUILDDIR)/$(notdir $(object)))) -o $(TARGETDIR)/$@ $(LDFLAGS)
	echo "running tests"
	$(RUN_FLAGS) $(TARGETDIR)/$@ $(GTEST_FLAGS)

$(filter %.o,$(TESTOBJECTS)): %.o : %.cc | check_dirs
	$(CXX) $(CXXFLAGS) -c $< -o $(foreach object,$@,$(BUILDDIR)/$(notdir $(object)))

#---------------------------------------------------------#

debug: CXXFLAGS += -DWITHOUT_MEMORY_LOCKING
debug: LDFLAGS += -fsanitize=address -g -fno-omit-frame-pointer -fno-optimize-sibling-calls
debug: RUN_FLAGS += $(shell ASAN_SYMBOLIZER_PATH=$(dpkg -L llvm-$(llvm-config  --version | grep [0-9][0-9] -o ) | grep -e /usr/lib.*symbolizer))
debug: test

valgrind: CXXFLAGS += -DWITHOUT_MEMORY_LOCKING -g
valgrind: test
	valgrind --tool=memcheck --leak-check=yes $(TARGETDIR)/$<

#---------------------------------------------------------#

gcov_report: COVFLAGS += --coverage
gcov_report: GCOV_FLAGS += -fprofile-arcs -ftest-coverage -fPIC -O0
gcov_report: clean test
	@mkdir -p $(GCOVDIR)
	@gcovr $(BUILDDIR) -r $(TARGETDIR) \
		--html --html-details \
		--exclude-unreachable-branches \
		--exclude-throw-branches \
		--exclude='.*\.h' \
		-o $(GCOVDIR)/coverage.html

lcov_report: COVFLAGS += --coverage -fprofile-arcs -ftest-coverage
lcov_report: GCOVFLAGS += -lgcov
lcov_report: clean test
	@mkdir -p $(LCOVDIR)
	@rm -f $(foreach object,$(TESTGCNO),$(BUILDDIR)/$(object))
	@lcov -t "Report" -c -d $(BUILDDIR) --rc lcov_branch_coverage=1 \
		--output-file $(BUILDDIR)/coverage.info \
		--exclude "*.h"
	@genhtml $(BUILDDIR)/coverage.info --rc lcov_branch_coverage=1 \
		--output-directory $(LCOVDIR)
# @open $(LCOVDIR)/index.html

#---------------------------------------------------------#

clang-n: # Prints style warnings
	clang-format --style=google -n $(TESTSDIR)/*/*.cc
	clang-format --style=google -n $(SOURCESDIR)/*/*.h
clang-i: # Fixes style warnings
	@clang-format --style=google -i $(TESTSDIR)/*.cc
	@clang-format --style=google -i $(SOURCESDIR)/*/*.h

.PHONY: clean all test debug valgrind gcov_report lcov_report clang-n clang-i rebuild

clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(GCOVDIR)
	@rm -rf $(LCOVDIR)
	@rm -f $(TARGETDIR)/test