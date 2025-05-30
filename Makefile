.PHONY: all tests clean

CXXFLAGS += -O3 -Wall -Wextra -Wpedantic -Werror -std=c++20 -I./include

T_APP = t_slen
T_SOURCES = slen.cpp $(T_APP).cpp
T_HEADER = slen.h string-literal.h

LIB = librequire.a
L_SOURCES = solid/require.cpp
L_OBJECTS = $(L_SOURCES:.cpp=.o)
L_HEADER = include/solid/require.h
MDP_RUN = mdp.run

all: $(MDP_RUN)
	@$(MAKE) --no-print-directory tests

%.o:%.cpp
	@echo "  building" $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(MDP_RUN): $(wildcard *.md)
	@echo "extracting source code"
	@[ -x "$$(command -v mdp)" ] || echo "mdp not installed" 1>&2
	@[ -x "$$(command -v mdp)" ] && mdp README.md
	@date >$@

tests: $(T_APP)
	@echo running unit-tests
	@./$(T_APP)


$(T_APP): $(T_SOURCES) $(T_HEADER) $(T_HEADER) $(LIB)
	@echo building $@
	$(CXX) $(CXXFLAGS) $(T_SOURCES) $(LIB) -o $@

$(LIB): $(L_OBJECTS)
	@echo building $@
	@$(AR) -cr $@ $^
	
clean:
	@echo "remove temporaries"
	@rm -f $(T_APP) $(MDP_RUN) $(LIB)
	@[ -x "$$(command -v mdp)" ] && rm -f $(T_SOURCES) $(L_SOURCES) \
		$(T_HEADER) $(L_HEADER) $(L_OBJECTS)
