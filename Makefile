include Makefile.MYFLTK

# HOW TO LINK
curveCombine: mainApp.o curveCombine.o mytree.o plotbox.o tools.o point.o
	$(CXX) -g $^ $(LIBS) -std=c++11 -o $@

mainApp.o: mainApp.cpp
	$(CXX) -g $(CXXFLAGS) -c $< -o $@

curveCombine.o: curveCombine.cpp
	$(CXX) -g $(CXXFLAGS) -c $< -o $@

mytree.o: mytree.cpp
	$(CXX) -g $(CXXFLAGS) -c $< -o $@

plotbox.o: plotbox.cpp
	$(CXX) -g $(CXXFLAGS) -c $< -o $@

tools.o: tools.cpp
	$(CXX) -g $(CXXFLAGS) -c $< -o $@

point.o: point.cpp
	$(CXX) -g $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o
