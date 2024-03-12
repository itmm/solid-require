.PHONY: test clean

CXXFLAGS += -Wall -std=c++20 -I./include

APP = t_strlen
SOURCES = solid/require.cpp strlen.cpp $(APP).cpp
HEADER = include/solid/require.h strlen.h string-literal.h
MDP_RUN = mdp.run

test: 
	$(MAKE) $(MDP_RUN)
	./$(APP)

mdp.run: README.md
	mdp $^ && date >$@ && $(MAKE) $(APP)

$(APP): $(SOURCES) $(HEADER)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(APP) $(MDP_RUN) $(SOURCES) $(HEADER)
