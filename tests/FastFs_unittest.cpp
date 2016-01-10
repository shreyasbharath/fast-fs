#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include "FastFs.h"

using ::testing::Eq;
using ::testing::StrEq;

class FastFsTest : public ::testing::Test {
protected:
    void SetUp() {
        m_pFastFs.reset( new FastFs() );
    }

    void TearDown() {
    }

    std::unique_ptr< FastFs > m_pFastFs;
};

TEST_F( FastFsTest, NoDirectoriesCreated_CurrentDirectoryReturnsRoot ) {
    EXPECT_THAT( m_pFastFs->CurrentDirectory(), StrEq( "/" ) );
}

TEST_F( FastFsTest, NoDirectoriesCreated_NumDirectoriesReturnsZero ) {
    EXPECT_THAT( m_pFastFs->NumDirectories(), Eq( 0 ) );
}

TEST_F( FastFsTest, DirectoryNotCreated_ExistsReturnsFalse ) {
    EXPECT_THAT( m_pFastFs->DirectoryExists( "test" ), Eq( false ) );
}

TEST_F( FastFsTest, DirectoryCreated_ExistsReturnsTrue ) {
    m_pFastFs->CreateDirectory( "test" );

    EXPECT_THAT( m_pFastFs->DirectoryExists( "test" ), Eq( true ) );
}

TEST_F( FastFsTest, DirectoryCreated_NumDirectoriesReturnsOne ) {
    m_pFastFs->CreateDirectory( "test" );

    EXPECT_THAT( m_pFastFs->NumDirectories(), Eq( 1 ) );
}

TEST_F( FastFsTest, DirectoryCreated_CreateDirectoryAgain_NumDirectoriesReturnsOne ) {
    m_pFastFs->CreateDirectory( "test" );

    m_pFastFs->CreateDirectory( "test" );

    EXPECT_THAT( m_pFastFs->NumDirectories(), Eq( 1 ) );
}

TEST_F( FastFsTest, DirectoryNotCreated_DeleteDirectory_ExistsReturnsFalse ) {
    m_pFastFs->DeleteDirectory( "test" );

    EXPECT_THAT( m_pFastFs->DirectoryExists( "test" ), Eq( false ) );
}

TEST_F( FastFsTest, DirectoryCreated_DeleteDirectory_ExistsReturnsFalse ) {
    m_pFastFs->CreateDirectory( "test" );

    m_pFastFs->DeleteDirectory( "test" );

    EXPECT_THAT( m_pFastFs->DirectoryExists( "test" ), Eq( false ) );
}

TEST_F( FastFsTest, DirectoryCreated_DeleteDirectory_NumDirectoriesReturnsZero ) {
    m_pFastFs->CreateDirectory( "test" );

    m_pFastFs->DeleteDirectory( "test" );

    EXPECT_THAT( m_pFastFs->NumDirectories(), Eq( 0 ) );
}

class FastFsDirTest : public ::testing::Test {
protected:
    void SetUp() {
        m_pFastFs.reset( new FastFs() );
        m_Dir = m_pFastFs->CreateDirectory( "test" );
    }

    void TearDown() {
    }

    std::unique_ptr< FastFs > m_pFastFs;
    std::shared_ptr< FastFsDirectory > m_Dir;
};

TEST_F( FastFsDirTest, DirectoryCreated_PathReturnsCorrectString ) {
    EXPECT_THAT( m_Dir->Path(), StrEq( "test" ) );
}

TEST_F( FastFsDirTest, DirectoryCreatedWithNoFiles_FileCountReturnsZero ) {
    EXPECT_THAT( m_Dir->FileCount(), Eq( 0 ) );
}

TEST_F( FastFsDirTest, DirectoryCreated_OpenFile_FileExistsReturnsTrue ) {
    m_Dir->OpenFile( "testfile", FastFsDirectory::FileOpenMode::OVERWRITE );

    EXPECT_THAT( m_Dir->FileExists( "testfile" ), Eq( true ) );
}

TEST_F( FastFsDirTest, DirectoryCreated_OpenFile_FileCountReturnsOne ) {
    m_Dir->OpenFile( "testfile", FastFsDirectory::FileOpenMode::OVERWRITE );

    EXPECT_THAT( m_Dir->FileCount(), Eq( 1 ) );
}

int main(int argc, char **argv) {
    // The following line must be executed to initialise Google Mock
    // (and Google Test) before running the tests.
    ::testing::InitGoogleMock(&argc, argv);

    int exitCode = -1;
    try {
        exitCode = RUN_ALL_TESTS();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    return exitCode;
}
