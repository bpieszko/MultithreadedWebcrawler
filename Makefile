all:
	g++-7 --std=c++17 -c MultithreadedWebcrawler.cpp -pthread -lcurl
	g++-7 --std=c++17 -o app.e MultithreadedWebcrawler.o main.cpp -pthread -lcurl

clean:
	rm *.e *.o