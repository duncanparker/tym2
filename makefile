CFLAGS = -std=c++11
LDFLAGS = -lmicrohttpd

tym2: src/main.cpp src/router.cpp
	g++ $(CFLAGS) -o tym2 src/main.cpp src/router.cpp $(LDFLAGS)

test: tym2
	./tym2

clean:
	rm -f tym2
