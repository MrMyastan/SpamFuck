#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <functional>
#include <stack>
#include <array>

// Am i using too many unsigned ints?
std::array<uint8_t, 30000> cells = {};
unsigned int pointer = 0;

unsigned int farthestCell = 0;

unsigned int lnNum = 1;
unsigned int colNum = 0;

int depth;

struct bracketLocation {
    std::streampos strmPos;
    unsigned int lnNum = 1;
    unsigned int colNum = 0;
};

bracketLocation preJumpLoc;

std::stack<bracketLocation> openingBrackets;

bool closingFound;

std::ifstream sourceCode;

void dumpCellValues();

void ptrOutOfBoundsErr();

// is this
bool add();
bool subtract();
bool left();
bool right();
bool read();
bool print();
bool open();
bool close();

std::string openCmd = "shallots";
std::string closeCmd = "aubergines";

std::map<std::string, std::function<bool(void)>> commands = {
    {"spam", add}, // +
    {"egg", subtract}, // -
    {"sausage", left}, // <
    {"bacon", right}, // >
    {"tomato", read}, // ,
    {"bakedbeans", print}, // .
    {"shallots", open}, // [
    {"aubergines", close} // ]
};
// to this a bit too boilerplate-y?

int main(int argc, char const* argv[])
{
    if (argc < 2) {
        std::cerr << "you need to pass a file name/path as the first argument";
        return EXIT_FAILURE;
    }

    sourceCode.open(argv[1]);
    if (!sourceCode.is_open()) {
        std::cerr << "couldn't open file, maybe the path or name given is wrong?";
        return EXIT_FAILURE;
    }

    char data;
    while (sourceCode.get(data)) {
        colNum++;
        if (data == '\n') {
            lnNum++;
            colNum = 0;
            continue;
        }

        std::streampos pos = sourceCode.tellg();

        for (const auto& kv : commands) {
            if (data != kv.first[0]) {
                continue;
            }

            int cmndLenMinusOne = kv.first.size() - 1;

            char buffer[10];
            sourceCode.read(buffer, cmndLenMinusOne);
            buffer[cmndLenMinusOne] = '\0';

            if (sourceCode.eof()) {
                sourceCode.clear();
                sourceCode.seekg(pos);
                continue;
            }

            // I'm aware that non zero values are true and so != 0 is redundant, but I feel this makes my intent more explicit
            if (kv.first.compare(1, cmndLenMinusOne, buffer) != 0) {
                sourceCode.seekg(pos);
                continue;
            }

            // if it wasn't the command it would've moved on already so we know it was this one
            bool result = kv.second();
            if (!result) {
                return EXIT_FAILURE;
            }
            colNum += sourceCode.gcount();
            break;
        }
    }
    if (openingBrackets.size() != 0) {
        std::cerr << "\nwell that's weird, we reached the end of program and theres still some open loops... The unclosed bracket/s are on:";
        for (int i = openingBrackets.size(); i > 0; i--) {
            std::cerr << "\n";
            bracketLocation errorBracket = openingBrackets.top();
            std::cerr << "Line: " << errorBracket.lnNum << ", Column: " << errorBracket.colNum;
            openingBrackets.pop();
        }
    }
    sourceCode.close();
    std::cout << "\n";
    dumpCellValues();
    return EXIT_SUCCESS;
}

bool add() {
    cells[pointer]++;
    return true;
}

bool subtract() {
    cells[pointer]--;
    return true;
}

bool left() {
    if (pointer == 0) { ptrOutOfBoundsErr(); return false; }
    pointer--;
    return true;
}

bool right() {
    if (pointer == cells.size() - 1) { ptrOutOfBoundsErr(); return false; }
    pointer++;
    if (pointer > farthestCell) { farthestCell = pointer; }
    return true;
}

bool read() {
    cells[pointer] = getchar();
    return true;
}

bool print() {
    std::cout << cells[pointer];
    return true;
}

bool open() {
    preJumpLoc = { sourceCode.tellg(), lnNum, colNum };
    if (cells[pointer] != 0) {
        openingBrackets.push(preJumpLoc);
        return true;
    }
    depth = 0;
    closingFound = false;
    char data;
    while (sourceCode.get(data)) {
        colNum++;
        if (data == 's' || data == 'a') {
            std::streampos pos = sourceCode.tellg();

            char buffer[10];
            sourceCode.read(buffer, 9);
            buffer[sourceCode.gcount()] = '\0';

            if (sourceCode.eof()) {
                sourceCode.clear();
                sourceCode.seekg(pos);
            }

            if (openCmd.compare(1, openCmd.size() - 1, buffer, openCmd.size() - 1) == 0) {
                depth++;
                colNum += sourceCode.gcount();
            }
            else if (closeCmd.compare(1, closeCmd.size() - 1, buffer, closeCmd.size() - 1) == 0) {
                if (depth == 0) {
                    closingFound = true;
                    colNum += sourceCode.gcount();
                    break;
                }
                depth--;
                colNum += sourceCode.gcount();
            } else {
                sourceCode.seekg(pos);
            }
        }
        else if (data == '\n') {
            lnNum++;
            colNum = 0;
        }
    }
    if (depth != 0 || !closingFound) {
        std::cerr << "\nmy guy you forgot to close a bracket pair thats not very gamer pog of you\n" << "Line: " << preJumpLoc.lnNum << ", Column: " << preJumpLoc.colNum << "\n";;
        dumpCellValues();
        return false;
    }
    return true;
}

bool close() {
    if (openingBrackets.size() == 0) {
        std::cerr << "\nYou just tried to use a closing bracket with no opening bracket, thats not very big brain\n" << "Line: " << lnNum << ", Column: " << colNum << "\n";
        dumpCellValues();
        return false;
    }
    if (cells[pointer] != 0) {
        sourceCode.seekg(openingBrackets.top().strmPos);
        lnNum = openingBrackets.top().lnNum;
        // the -2 makes it so when the length-1 of aubergines is added after 
        // the command is executed that the colnum indicator won't be off
        // its -2 not -1 because there will be another increment of colnum
        // once it rolls around to the next command
        colNum = openingBrackets.top().colNum - 2;
        return true;
    }
    openingBrackets.pop();
    return true;
}

void ptrOutOfBoundsErr() {
    std::cerr << "\nbro you just tried to go out side the cell array bounds, thats like, kinda cring\n" << "Line: " << lnNum << ", Column: " << colNum << "\n";;
    dumpCellValues();
}

int recursionCount = 0;

void dumpCellValues() {
    std::cout << "Do you want to dump the memory cell values? y/n: ";
    std::string input;
    std::cin >> input;
    if (input == "y" || input == "yes") {
        for (unsigned int i = 0; i <= farthestCell; i++) {
            std::cout << "Cell " << i << ": " << +cells[i] << "\n";
        }
        std::cout << "Pointer is at cell " << pointer;
    }
    else if (input == "n" || input == "no") {
        std::cout << "coolio";
    }
    else {
        if (recursionCount < 4) {
            recursionCount++;
            dumpCellValues();
        }
        else {
            std::cout << "You are either really bad at typing or ABSOLUTELY HILARIOUS";
        }
    }
    char wait;
    std::cin >> wait;
    return;
}