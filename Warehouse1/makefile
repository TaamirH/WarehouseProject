all: compile link clean

clean:
	rm -f ./bin/*

compile:  # Added colon here
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o ./bin/file1.o ./src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o ./bin/file2.o ./src/warehouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o ./bin/file3.o ./src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o ./bin/file4.o ./src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o ./bin/file6.o ./src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o ./bin/file7.o ./src/Action.cpp

link:
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -o ./warehouse ./bin/*.o