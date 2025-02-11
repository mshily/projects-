#include "archiver.h"

int main(int argc, char **argv) {

    Reader reader(argc, argv);

    if (reader.is_compress) {
        Compress(reader.archive_name, reader.file_pathes);
    } else if (reader.is_decompress) {
        Decompress(reader.archive_name);
    } else if (reader.is_log) {
        std::cerr << "archiver -c archive_name file1 [file2 ...] - заархивировать файлы "
                     "file1, file2, ... и сохранить результат в файл archive_name.\n"
                     "\n"
                     "archiver -d archive_name - разархивировать файлы из архива "
                     "archive_name и положить в текущую директорию.\n"
                     "\n"
                     "archiver -h - вывести справку по использованию программы."
                  << std::endl;
    }
    return 0;
}
