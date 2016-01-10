#ifndef FAST_FS_H_
#define FAST_FS_H_

#include <cstdint>
#include <string>
#include <map>
#include <memory>
#include "FastFsDirectory.h"

class FastFs {
public:
    FastFs();
    ~FastFs();

    std::shared_ptr< FastFsDirectory > CreateDirectory( const std::string& dirPath );
    void DeleteDirectory( const std::string& dirPath );
    bool DirectoryExists( const std::string& dirPath ) const;
    std::string CurrentDirectory() const;
    void ChangeDirectory( const std::string& dirPath );
    uint32_t NumDirectories() const;

private:
    std::map< std::string, std::shared_ptr< FastFsDirectory > > m_DirectoryMap;
    std::string m_CurrentDir;
};

#endif // FAST_FS_H_
