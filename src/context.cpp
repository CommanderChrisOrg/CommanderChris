#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/config.hpp>
#include "context.hpp"

using namespace boost::filesystem;

std::string get_path_string(path p){
    std::string ret;

    for (auto it = p.begin(); it != p.end(); ++it) {
        if (it->string().find(' ') != std::string::npos) ret += "'" + it->string() + "'";
        else ret += it->string();
        if (it != p.begin()) ret += "/";
    }

    return ret;
}

std::string get_dir_files(path p){
    std::string ret = "";
    if(is_directory(p)) {
        int count = 0;
        auto it = directory_iterator(p);
        auto end = directory_iterator();
        for (;it != end && count < 15; ++it, ++count) ret += get_path_string(it->path()) + "\n";
    }
    return ret;
}

std::string get_file_context(){
    path p;
    std::string ret = "";

    p = current_path().parent_path().parent_path();
    ret += "files and directories in " + get_path_string(p) + ":\n" + get_dir_files(p) + "\n";

    p = current_path().parent_path();
    ret += "files and directories in " + get_path_string(p) + ":\n" + get_dir_files(p) + "\n";

    p = current_path();
    ret += "files and directories in current directory (" + get_path_string(p) + "):\n" + get_dir_files(p); 

    return ret;
}

#ifdef BOOST_WINDOWS_API
#include <windows.h>
#else
#include <sys/utsname.h>
#endif

std::string get_os_context(){
    std::stringstream ss;
    #ifdef BOOST_WINDOWS_API
        OSVERSIONINFOEX osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        GetVersionEx((OSVERSIONINFO*)&osvi);
        ss << "Operating System: Windows " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << std::endl;
        return ss.str();
    #else
        struct utsname unameData;
        if (uname(&unameData) == 0) {
            ss << "Operating System: " << unameData.sysname << " " << unameData.release << std::endl;
            return ss.str();
        }
        return "Operating System: POSIX\n";
    #endif
}

std::string get_context(){ return get_os_context() + "\n" + get_file_context(); }
