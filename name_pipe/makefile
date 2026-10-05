all : client service
client : Client.cpp
	g++ -o $@ $^ -std=c++11
service : Service.cpp
	g++ -o $@ $^ -std=c++11

.PHONY : clean
clean:
	rm -rf client service