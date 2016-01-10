//
// Created by Shreyas on 10/01/2016.
//

#ifndef FAST_FS_FASTFSFILE_H
#define FAST_FS_FASTFSFILE_H

#include <cstdint>
#include <string>
#include <vector>

enum class FileOpenMode { OVERWRITE, APPEND };

class FastFsFile {
public:
    FastFsFile( const std::string& filePath, FileOpenMode mode );
    ~FastFsFile();

    std::string Filename() const;
    const std::vector< uint8_t >& Read();
    void Write(const std::vector< uint8_t >& data);
    void Seek( size_t seekPos );
    size_t Size();

private:
    std::string m_FilePath;
    FileOpenMode m_Mode;
    std::vector< uint8_t > m_Data;
};

#endif //FAST_FS_FASTFSFILE_H
