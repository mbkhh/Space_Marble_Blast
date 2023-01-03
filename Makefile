all:
	g++ main.cpp -I C:/SDL2-w64/include -L C:/SDL2-w64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -static-libgcc -static-libstdc++  -o bin/main
	del /S /Q bin\assest\*
	xcopy assest bin\assest /E /H /C /I
	./bin/main.exe
	