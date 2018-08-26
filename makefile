CFLAGS = -std=c++11
LDFLAGS = -lmicrohttpd

tym2: main.cpp router.cpp
	g++ $(CFLAGS) -o tym2 main.cpp router.cpp $(LDFLAGS)

test: tym2
	./tym2

clean:
	rm -f tym2
