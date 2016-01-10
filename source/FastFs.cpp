#include <algorithm>
#include "FastFs.h"

// TODO Create a path class to do path validation logic - invalid chars (on both Linux and Win etc.)

FastFs::FastFs() :
    m_DirectoryMap { },
    m_CurrentDir { "/" }
{
}

FastFs::~FastFs() {
}

std::shared_ptr< FastFsDirectory > FastFs::CreateDirectory( const std::string& dirPath ) {
    if ( DirectoryExists( dirPath ) ) {
        return m_DirectoryMap[ dirPath ];
    }
    auto dir = std::make_shared< FastFsDirectory >( dirPath );
    m_DirectoryMap[ dirPath ] = dir;
    return dir;
}

void FastFs::DeleteDirectory( const std::string& dirPath ) {
    if ( !DirectoryExists( dirPath ) ) {
        return;
    }
    m_DirectoryMap.erase( dirPath );
}

bool FastFs::DirectoryExists( const std::string& dirPath ) const {
    bool dirExists = m_DirectoryMap.find( dirPath ) != m_DirectoryMap.end();
    return dirExists;
}

std::string FastFs::CurrentDirectory() const {
    return m_CurrentDir;
}

void FastFs::ChangeDirectory( const std::string& dirPath ) {

}

uint32_t FastFs::NumDirectories() const {
    return m_DirectoryMap.size();
}
