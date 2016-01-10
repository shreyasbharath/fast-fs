//
// Created by Shreyas on 10/01/2016.
//

#include "FastFsFile.h"

FastFsFile::FastFsFile( const std::string& path ) {

}

FastFsFile::~FastFsFile() {

}

const std::vector< uint8_t >& FastFsFile::Read() {
    return m_Data;
}

void FastFsFile::Write(const std::vector< uint8_t >& data) {
    m_Data = data;
}

size_t FastFsFile::Size() {
    return m_Data.size();
}
