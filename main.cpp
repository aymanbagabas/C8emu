#include "src/Emulator.h"
#include <iostream>
#include <string>
#include <fstream>

int load(string file) {
    // read file
    ifstream f(file, ios::in|ios::binary|ios::ate);
    if (!f.good()) {
        cout << "Couldn't read '" << file << "'!" << endl;
        return 1;
    }

    return 0;
}

int main(int argc, char** argv) {
    if (argc >= 2) {
        if (!load(string(argv[argc-1]))) {
            Emulator *emu = new Emulator();
            for (int i = 0; i < argc; i++) {
                switch (argv[i][1]) {
                    case 'v':
                        emu->setDebug();
                    default:
                        break;
                }
            }
            emu->run(string(argv[argc - 1]));
            delete emu;
        }
    } else {
        cout << argv[0] << " [ROM FILE]" << endl;
        return 1;
    }
    return 0;
}
