## Mania Conversion
Mania Conversion converts [standard osu! maps](https://www.youtube.com/watch?v=pBgn--Ve2WU) into a different mode similar to 'Beatmania'. This game reads [.osu file format](https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osu_(file_format)) and instantiates notes in a 'mania' format based on the x position (horizontal) of the note in standard mode. Users can convert their standard osu! beatmap and then play it.

## Dependencies
```
SDL2
SDL2_mixer
```

## Build and Run
1. Add .osu file and beatmap audio to directory
2. Edit `fileLocation` in `source.cpp` with .osu file location
3. Edit `gMusic` in `source.cpp` by inserting beatmap audio location as an argument in `Mix_LoadMUS("")`
3. Compile and run:
```
$ g++ source.cpp -w -lSDL2main -lSDL2 -lSDL2_mixer -o source
$ ./source
```