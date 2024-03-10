.PHONY: test clean

CXXFLAGS += -Wall -std=c++20

test: 
	$(MAKE) mdp.run
	./t_strlen

mdp.run:  assert-problems.md
	mdp $^
	date >$@
	$(MAKE) t_strlen

t_strlen: strlen.h strlen.cpp t_strlen.cpp
	$(CXX) $(CXXFLAGS) strlen.cpp t_strlen.cpp -o $@

clean:
	rm -f t_strlen mdp.run
