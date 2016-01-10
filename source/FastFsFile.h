//
// Created by Shreyas on 10/01/2016.
//

#ifndef FAST_FS_FASTFSFILE_H
#define FAST_FS_FASTFSFILE_H

#include <cstdint>
#include <string>
#include <vector>

class FastFsFile {
public:
    FastFsFile( const std::string& path );
    ~FastFsFile();

    const std::vector< uint8_t >& Read();
    void Write(const std::vector< uint8_t >& data);
    size_t Size();

private:
    std::vector< uint8_t > m_Data;
};

#endif //FAST_FS_FASTFSFILE_H
