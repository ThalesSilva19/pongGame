all: compile

compile:
	g++ server.cpp -pthread -o server
	g++ client.cpp -pthread -o client

server: compile
	./server

client: compile
	./client

clean:
	rm server
	rm client
