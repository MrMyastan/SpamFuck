# SpamFuck
An interpreter for my Brainfuck-equivalent language, SpamFuck.
## Commands
All of the commands are ingredients from Monty Python Spam Sketch meals
Command | Brainfuck equivalent
------- | --------------------
spam | +
egg | -
sausage | <
bacon | >
tomato | ,
bakedbeans | .
shallots | \[
aubergines | ]
## Usage
- The commands are case-sensitive, all lower case.
- Anything that is not a full formed command will be ignored. (So add some comments, just don't say spam!)
- No, you cannot have a space in bakedbeans.
## My Implementation
Yeah yeah, the code is a mess, maybe I'll get around to cleaning it up a bit and making an AST type thing, idk. I haven't thoroughly debugged it but I'm fairly confident in it, I got a ~~find and replace'd~~ transpiled version of [mitxela's bf tic tac toe ai](https://github.com/mitxela/bf-tic-tac-toe) working just fine in it, along with some Hello World's. I got all the issues I noticed hammered out (misreporting line numbers, not finding closing brackets) but if you find anything feel free to open an issue and I'll give it a fix!

While writing this interpreter, I ran into this weird issue on uh... `g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0` <- this version of MinGW g++ where if there was a newline in a file `std::ifstream`'s  `.tellg()` would report num-of-newlines ahead of where it should. Except when you haven't read any characters in, then it reports 0 as usual. Also, if you `.seekg()` back to this over-reported number you end up at that over-reported point, so ahead of where you were when you `.tellg()`'d. Once you reach the newlines it starts syncing back up though. This issue (at least I think it's an issue, maybe I just don't understand the intended behaviors?) meant that I had to reinstall Visual Studio *gasp* to get MSVC, where it was working as I expected. ~~Honestly I prefer Visual Studio's C++ integration over VSCode's.~~

If you are wondered about any implementation details like how many cells there are or how it treats EOF, check [my brainfuck interpreter](https://github.com/MrMyastan/cpp-brainfuck-interpreter), under the hood it's the same thing.
## Implementing SpamFuck
As long as the commands are the same, you're good to go! I got my info about how brainfuck works from [here](https://esolangs.org/wiki/Brainfuck)  
