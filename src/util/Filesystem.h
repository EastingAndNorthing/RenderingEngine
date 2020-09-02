#pragma once

#include <fstream>
#include <string>
#include <sstream>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

class Filesystem {
public:

	Filesystem(Filesystem const&) = delete;
  	~Filesystem() = default;

	static Filesystem& Instance();

    std::string basePath;

    std::string getBasePath();
    
    std::string getFileContents(const std::string& relativePath);

private:

	Filesystem();

};
