//
// Created by Shreyas on 10/01/2016.
//

#ifndef FAST_FS_FASTFSDIRECTORY_H
#define FAST_FS_FASTFSDIRECTORY_H

// TODO Support nested directories
// TODO CloseFile API required? Does not seem to make sense

#include <cstdint>
#include <string>
#include <map>
#include <memory>
#include "FastFsFile.h"

class FastFsDirectory {
public:
    FastFsDirectory( const std::string& dirPath );
    ~FastFsDirectory();

    enum class FileOpenMode { OVERWRITE, APPEND };

    std::string Path();
    std::shared_ptr< FastFsFile > OpenFile(const std::string&filename, FileOpenMode mode );
    bool FileExists( const std::string& filePath );
    void RenameFile( const std::string& oldFile, const std::string& newFile );
    void DeleteFile( const std::string& filePath );
    uint32_t FileCount() const;
    std::map< std::string, std::shared_ptr< FastFsFile > > Listing();
    size_t Size() const;

    FastFsDirectory& operator=( FastFsDirectory other );
    bool operator==( const FastFsDirectory& other ) const;
    bool operator!=( const FastFsDirectory& other ) const;

private:
    std::string m_Path;
    std::map< std::string, std::shared_ptr< FastFsFile > > m_FileMap;
};

#endif //FAST_FS_FASTFSDIRECTORY_H
