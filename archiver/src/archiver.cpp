#include "archiver.h"

int main(int argc, char **argv) {

    Reader reader(argc, argv);

    if (reader.is_compress) {
        Compress(reader.archive_name, reader.file_pathes);
    } else if (reader.is_decompress) {
        Decompress(reader.archive_name);
    } else if (reader.is_log) {
        std::cerr << "archiver -c archive_name file1 [file2 ...] - zip files "
                     "file1, file2, ... and save the result to a file archive_name.\n"
                     "\n"
                     "archiver -d archive_name - unzip the files in the archive "
                     "archive_name and put it in the current directory.\n"
                     "\n"
                     "archiver -h - display help on how to use the program."
                  << std::endl;
    }
    return 0;
}
