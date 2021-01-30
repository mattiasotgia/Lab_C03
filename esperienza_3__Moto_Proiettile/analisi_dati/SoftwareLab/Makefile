all: minimiquadrati media

FLAGS = -Wall -Wno-sign-compare
#FLAGS += `root-config --cflags --libs`

media: media.cpp
	g++ -o media media.cpp $(FLAGS)

minimiquadrati: minimiquadrati.cpp
	g++ -o minimiquadrati minimiquadrati.cpp $(FLAGS)

clean:
	rm -f *~ media minimiquadrati

