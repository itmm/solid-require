.PHONY: test clean

CXXFLAGS += -Wall -std=c++20

SOURCES = strlen.cpp t_strlen.cpp
HEADER = strlen.h
APP = t_strlen
MDP_RUN = mdp.run

test: 
	$(MAKE) $(MDP_RUN)
	./$(APP)

mdp.run:  README.md
	mdp $^
	date >$@
	$(MAKE) $(APP)

t_strlen: $(SOURCES) $(HEADER)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(APP) $(MDP_RUN)
