CFLAGS = -std=c++11 -I/usr/include/postgresql
LDFLAGS = -lmicrohttpd -lpq

tym2: src/main.cpp src/router.cpp src/pgdataprovider.cpp
	g++ $(CFLAGS) -o tym2 src/main.cpp src/router.cpp src/pgdataprovider.cpp $(LDFLAGS)

test: tym2
	./tym2

clean:
	rm -f tym2
