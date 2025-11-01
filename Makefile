BINARY := bin
INCLUDE := ./include/
CODEDIRS := . ./src

CC := g++ 
CFLAGS = -Wall -Wextra -I$(INCLUDE) 

CFILES = $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.cpp))
OFILES = $(patsubst %.cpp,%.o,$(CFILES))

target: $(BINARY)

$(BINARY): $(OFILES)
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.cpp  
	$(CC) $(CFLAGS) -c $< -o $@ 

clean: 
	rm -rf bin *.o src/*.o *.json 
