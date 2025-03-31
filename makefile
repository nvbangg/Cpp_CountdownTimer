all:
	g++ -Isrc/Include -Lsrc/lib -o main main.cpp functions.cpp draw.cpp input_time.cpp countdown.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf	
	
run:
	./main
	