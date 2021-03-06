//
// Created by Shreyas on 10/01/2016.
//

#include "FastFsDirectory.h"

FastFsDirectory::FastFsDirectory( const std::string& dirPath ) :
    m_Path( dirPath ),
    m_FileMap { }
{
}

FastFsDirectory::~FastFsDirectory() {
}

std::string FastFsDirectory::Path() {
    return m_Path;
}

std::shared_ptr< FastFsFile > FastFsDirectory::OpenFile(const std::string& filename, FileOpenMode mode ) {
    auto file = std::make_shared< FastFsFile >( filename );
    m_FileMap[ filename ] = file;
    return file;
}

bool FastFsDirectory::FileExists( const std::string& filePath ) {
    bool fileExists = m_FileMap.find( filePath ) != m_FileMap.end();
    return fileExists;
}

uint32_t FastFsDirectory::FileCount() {
    return m_FileMap.size();
}

FastFsDirectory& FastFsDirectory::operator=( FastFsDirectory other ) {
    m_Path = other.m_Path;
    m_FileMap = other.m_FileMap;
    return *this;
}

bool FastFsDirectory::operator==( const FastFsDirectory& other ) const {
    return ( m_Path == other.m_Path );
}

bool FastFsDirectory::operator!= ( const FastFsDirectory& other ) const {
    return !operator==( other );
}
