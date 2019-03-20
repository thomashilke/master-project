CXXFLAGS= -Wall -ansi -pedantic -Wextra -g -I../

TARGETS= testsmartptr testrenderer testsingleton testtiles testtuples testtiles2 testmatrix

all: $(TARGETS)

testsmartptr: testsmartptr.cpp ../utils/smartptr.hpp
	g++ -o $@ $< $(CXXFLAGS)

testrenderer: testrenderer.cpp ../renderer/renderer.hpp \
 ../renderer/x11utilities.hpp ../renderer/eventmanager.hpp \
 ../utils/smartptr.hpp ../utils/tinymatrix/vector.hpp \
 ../renderer/openglexts.hpp ../renderer/glext.h ../utils/singleton.hpp \
 ../utils/smartptr.hpp ../utils/tinymatrix/matrix.hpp \
 ../utils/tinymatrix/vector.hpp ../renderer/texture.hpp \
 ../renderer/vertexbuffer.hpp
	g++ -o $@ $< $(CXXFLAGS) ../renderer/*.cpp -lGL -lX11

testsingleton: testsingleton.cpp ../utils/singleton.hpp \
 ../utils/smartptr.hpp
	g++ -o $@ $< $(CXXFLAGS)

testtiles: testtiles.cpp ../tile/tile.hpp ../utils/smartptr.hpp \
 ../tile/videostoragefactory.hpp ../utils/singleton.hpp ../utils/smartptr.hpp \
 ../tile/matrixstorage.hpp ../tile/diskstoragefactory.hpp \
 ../utils/stringbuilder.hpp ../tile/diskstorage.hpp ../tile/diskstorage.hpp \
 ../tile/videostorage.hpp ../tile/centralstorage.hpp
	g++ -o $@ $< $(CXXFLAGS) ../renderer/*.cpp -lGL -lX11

testtiles2: testtiles2.cpp ../tile/tile.hpp ../utils/smartptr.hpp \
 ../tile/videostoragefactory.hpp ../utils/singleton.hpp ../utils/smartptr.hpp \
 ../tile/matrixstorage.hpp ../tile/diskstoragefactory.hpp \
 ../utils/stringbuilder.hpp ../tile/diskstorage.hpp ../tile/diskstorage.hpp \
 ../tile/videostorage.hpp ../tile/centralstorage.hpp
	g++ -o $@ $< $(CXXFLAGS) ../renderer/*.cpp -lGL -lX11

testtuples: testtuples.cpp ../utils/tuple.hpp
	g++ -o $@ $< $(CXXFLAGS)


testmatrix: testmatrix.cpp ../utils/tinymatrix/matrix.hpp
	g++ -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(TARGETS)