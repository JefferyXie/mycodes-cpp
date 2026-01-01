#include "teris.h"

int main(int argc, char** argv)
{
    std::unique_ptr<input_reader_t> reader;
    if (argc > 1) {
        std::cout << "Input file: " << argv[1] << std::endl;
        reader = std::make_unique<input_reader_t>(argv[1]);
    } else {
        std::cout << "Waiting for your input: empty input line, Ctrl+D, or Ctrl+Z will exit the app..." << std::endl;
        reader = std::make_unique<input_reader_t>(std::cin);
    }

    constexpr int Max_Grid_Width  = 10;
    constexpr int Max_Grid_Height = 200;

    teris_engine_t engine{Max_Grid_Width, Max_Grid_Height};
    engine.set_reader(std::move(reader)).run();

    return 0;
}
