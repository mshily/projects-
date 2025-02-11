// Программа-архиватор должна иметь следующий интерфейс командной строки:
//* `archiver -c archive_name file1 [file2 ...]` - заархивировать файлы `file1,
// file2, ...` и сохранить результат в файл `archive_name`.
//* `archiver -d archive_name` - разархивировать файлы из архива `archive_name`
// и положить в текущую директорию.
//* `archiver -h` - вывести справку по использованию программы.

#include "archiver.h"

Reader::Reader(int argc, char **argv) {
    // argv[0] = command name
    // argv[1] - flag of used
    if (argc == 1) {
        std::cerr << "argc cant be 1" << std::endl;
        exit(ERROR);
    }

    std::string flag = argv[1];
    if (flag == "-c") {
        Reader::is_compress = true;
        // argv[2] - name of file
        // argv[3..n] - files to compress

        if (argc < 4) {
            std::cerr << "argc cant be < 4 in -c" << std::endl;

            exit(ERROR);
        }

        Reader::archive_name = argv[2];
        for (int id = 3; id < argc; ++id) {
            file_pathes.emplace_back(argv[id]);
        }

    } else if (flag == "-d") {
        Reader::is_decompress = true;

        // argv[2] - name of file

        if (argc < 3) {
            std::cerr << "argc cant be < 2 in -d" << std::endl;

            exit(ERROR);
        }

        Reader::archive_name = argv[2];
    } else if (flag == "-h") {
        Reader::is_log = true;
    } else {
        std::cerr << "invaluable argument!" << std::endl;
        exit(ERROR);
    }
}