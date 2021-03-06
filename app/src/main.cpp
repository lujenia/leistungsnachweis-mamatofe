#include <iostream>
#include <args.hxx>

#include "suso.h"


int main(int argc, const char *argv[]) {
    Sudoku sudoku{};

    std::unordered_map<std::string, Mode> map{
            {"default",                  Mode::DEFAULT},
            {"naked",                    Mode::NAKED},
            {"hidden",                   Mode::HIDDEN},
            {"backtracking",             Mode::BACKTRACKING},
            {"lrb",                      Mode::LAST_RESORT_BACKTRACKING},
            {"last_resort_backtracking", Mode::LAST_RESORT_BACKTRACKING}};

    args::ArgumentParser parser("Solves the classical Sudoku game.");
    parser.LongSeparator(" ");
    args::HelpFlag help(parser, "help", "Display this help", {'h', "help"});
    args::MapFlag<std::string, Mode> algo(parser, "algorithm",
                                          "The algorithm that should be used to solve the Sudoku. "
                                          "Possible values are: default, naked, hidden, backtracking, lrb (last_resort_backtracking)",
                                          {'a', "algo"}, map);
    args::Positional<std::string> file(parser, "file", "The file containing the Sudoku to solve");

    try {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help&) {
        std::cout << parser;
        return EXIT_SUCCESS;
    }
    catch (args::ParseError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }

    if (!file) {
        std::cout << "You need to give a file containing the Sudoku to solve." << std::endl;
        return EXIT_FAILURE;
    }

    if (!sudoku.loadFromFile(args::get(file))) {
        std::cout << "Failed to load the data from file '" + args::get(file) + "'" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loaded Sudoku:" << std::endl << sudoku << std::endl;

    try {
        sudoku.checkSolvability();
        sudoku.solve(args::get(algo));
        std::cout << "Solved Sudoku:" << std::endl << sudoku << std::endl;
        std::cout << sudoku.getSolvingMetrics() << std::endl;
    }
    catch (const std::runtime_error &e) {
        std::cout << "Sudoku could not be solved:" << std::endl;
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}