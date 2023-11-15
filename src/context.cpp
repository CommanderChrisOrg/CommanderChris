#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "context.hpp"

using namespace boost::filesystem;

std::string get_dir_files(path p){
    std::string ret = "";
    if(is_directory(p)) {
        int count = 0;
        auto it = directory_iterator(p);
        auto end = directory_iterator();
        for (;it != end && count < 15; ++it, ++count) ret += it->path().filename().string() + "\n";
    }
    return ret;
}

std::string get_file_context(){
    path p;
    std::string ret = "";

    p = current_path().parent_path().parent_path();
    ret += "files and directories in " + p.string() + ":\n" + get_dir_files(p) + "\n\n";

    p = current_path().parent_path();
    ret += "files and directories in " + p.string() + ":\n" + get_dir_files(p) + "\n\n";

     p = current_path();
    ret += "files and directories in current directory " + p.string() + ":\n" + get_dir_files(p) + "\n\n"; 

    return ret;
}