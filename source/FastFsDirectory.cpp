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
    auto file = std::make_shared< FastFsFile >( filename, mode );
    m_FileMap[ filename ] = file;
    return file;
}

bool FastFsDirectory::FileExists( const std::string& filePath ) const {
    bool fileExists = m_FileMap.find( filePath ) != m_FileMap.end();
    return fileExists;
}

void FastFsDirectory::DeleteFile( const std::string& filePath ) {
    if ( !FileExists( filePath ) ) {
        return;
    }
    m_FileMap.erase( filePath );
}

uint32_t FastFsDirectory::FileCount() const {
    return m_FileMap.size();
}

size_t FastFsDirectory::Size() const {
    size_t totalSize = 0;
    for ( auto const file : m_FileMap ) {
        totalSize += file.second->Size();
    }
    return totalSize;
}

std::vector< std::string > FastFsDirectory::Listing() const {
    std::vector< std::string > files;
    for ( auto file : m_FileMap ) {
        files.push_back( file.first );
    }
    return files;
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
