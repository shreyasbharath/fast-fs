//
// Created by Shreyas on 10/01/2016.
//

#include "FastFsFile.h"

FastFsFile::FastFsFile( const std::string& filePath, FileOpenMode mode ) :
    m_FilePath( filePath ),
    m_Mode( mode )
{
}

FastFsFile::~FastFsFile() {
}

const std::vector< uint8_t >& FastFsFile::Read() {
    return m_Data;
}

void FastFsFile::Write(const std::vector< uint8_t >& data) {
    if ( m_Mode == FileOpenMode::OVERWRITE ) {
        m_Data = data;
    }
    else {
        m_Data.insert( std::end( m_Data ), std::begin( data ), std::end(  data) );
    }
}

size_t FastFsFile::Size() {
    return m_Data.size();
}
