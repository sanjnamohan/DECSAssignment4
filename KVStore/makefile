all: KVServer KVClient

KVServer:KVServer.cpp KVServer.h
	g++ $^ -lpthread -o $@

KVClient:KVClient.cpp
	g++ $^ -lpthread -o $@

runServer:KVServer
	./$<

runClient:KVClient
	./$<