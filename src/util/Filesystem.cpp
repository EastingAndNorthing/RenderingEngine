#include "common.h"
#include "util/Filesystem.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif _WIN32
#include <Windows.h>
#endif

Filesystem::Filesystem() {
    basePath = Filesystem::getBasePath();
}

Filesystem& Filesystem::Instance() {
    static Filesystem instance;
    return instance;
}

std::string Filesystem::getBasePath() {
      
    #ifdef __APPLE__
        char c_path[1024];
        uint32_t size = sizeof(c_path);
        if (_NSGetExecutablePath(c_path, &size) == 0)
            std::string path(c_path);
        else
            printf("Could not get executable path, I need %u characters\n", size);
        
        std::string path(c_path);

        std::string executable = "main.app";

    #elif _WIN32
        // Copied from Boost library
        typedef std::vector<char> char_vector;
        typedef std::vector<char>::size_type size_type;
        char_vector buf(1024, 0);
        size_type size = buf.size();
        bool havePath = false;
        bool shouldContinue = true;
        do {
            DWORD result = GetModuleFileNameA(nullptr, &buf[0], size);
            DWORD lastError = GetLastError();
            if (result == 0)
            {
                shouldContinue = false;
            } else if (result < size) {
                havePath = true;
                shouldContinue = false;
            }
            else if (
                result == size
                && (lastError == ERROR_INSUFFICIENT_BUFFER || lastError == ERROR_SUCCESS)
            ) {
                size *= 2;
                buf.resize(size);
            } else {
                shouldContinue = false;
            }
        } while (shouldContinue);

        std::string path = &buf[0];

    #endif

    // Make relative
    // std::string::size_type i = path.find(executable);
    // if (i != std::string::npos)
    //     path.erase(i, executable.length());

    path = path.substr(0, path.find_last_of("/\\")); // Make relative

    return path;
}

std::string Filesystem::getFileContents(const std::string& relativePath) {
    
    // MacOS "main.app.dSYM/Contents/..."
    std::ifstream stream(basePath + relativePath);
    std::string contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    
    if (!stream.is_open()) {
        printf("[FS ERROR] File '%s' could not be opened.\n", relativePath.c_str());
        return 0;
    }

    return contents;
}
