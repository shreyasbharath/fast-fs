#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include "FastFs.h"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::ContainerEq;
using ::testing::ElementsAre;

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
    m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );

    EXPECT_THAT( m_Dir->FileExists( "testfile" ), Eq( true ) );
}

TEST_F( FastFsDirTest, DirectoryCreated_OpenFile_FileCountReturnsOne ) {
    m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );

    EXPECT_THAT( m_Dir->FileCount(), Eq( 1 ) );
}

TEST_F( FastFsDirTest, FileAlreadyOpened_OpenFileAgain_FileCountReturnsOne ) {
    m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );

    m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );

    EXPECT_THAT( m_Dir->FileCount(), Eq( 1 ) );
}

TEST_F( FastFsDirTest, FileOpened_AnotherFileOpened_FileCountReturnsTwo ) {
    m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );

    m_Dir->OpenFile( "testfile2", FileOpenMode::OVERWRITE );

    EXPECT_THAT( m_Dir->FileCount(), Eq( 2 ) );
}

TEST_F( FastFsDirTest, FileNotOpened_DeleteFile_FileCountReturnsZero ) {
    m_Dir->DeleteFile( "testfile" );

    EXPECT_THAT( m_Dir->FileCount(), Eq( 0 ) );
}

TEST_F( FastFsDirTest, FileOpened_DeleteFile_FileCountReturnsZero ) {
    m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );

    m_Dir->DeleteFile( "testfile" );

    EXPECT_THAT( m_Dir->FileCount(), Eq( 0 ) );
}

TEST_F( FastFsDirTest, FileOpened_DeleteFile_FileExistsReturnsFalse ) {
    m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );

    m_Dir->DeleteFile( "testfile" );

    EXPECT_THAT( m_Dir->FileExists( "testfile" ), Eq( false ) );
}

TEST_F( FastFsDirTest, FileOpened__FileExistsReturnsFalse ) {
    m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );

    m_Dir->DeleteFile( "testfile" );

    EXPECT_THAT( m_Dir->FileExists( "testfile" ), Eq( false ) );
}

TEST_F( FastFsDirTest, EmptyDirectoryCreated_DirSizeReturnsZero ) {
    EXPECT_THAT( m_Dir->Size(), Eq( 0 ) );
}

TEST_F( FastFsDirTest, DirectoryCreated_TwoFilesOpenedAndWrittenTo_DirSizeReturnsCumulativeSizeOfFiles ) {
    auto file1 = m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );
    std::vector< uint8_t > file1Data { 0, 1, 2, 3 };
    file1->Write( file1Data );
    auto file2 = m_Dir->OpenFile( "testfile2", FileOpenMode::OVERWRITE );
    std::vector< uint8_t > file2Data { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    file2->Write( file2Data );

    EXPECT_THAT( m_Dir->Size(), Eq( file1Data.size() + file2Data.size() ) );
}

TEST_F( FastFsDirTest, DirectoryCreated_TwoFilesOpened_DirListingListsFiles ) {
    auto file1 = m_Dir->OpenFile( "testfile", FileOpenMode::OVERWRITE );
    auto file2 = m_Dir->OpenFile( "testfile2", FileOpenMode::OVERWRITE );

    EXPECT_THAT( m_Dir->Listing(), ElementsAre( "testfile", "testfile2" ) );
}

class FastFsFileTest : public ::testing::Test {
protected:
    void SetUp() {
        m_pFastFs.reset( new FastFs() );
        m_pDir = m_pFastFs->CreateDirectory( "test" );
        m_pFile = m_pDir->OpenFile( "testfile", FileOpenMode::OVERWRITE );
    }

    void TearDown() {
    }

    std::unique_ptr< FastFs > m_pFastFs;
    std::shared_ptr< FastFsDirectory > m_pDir;
    std::shared_ptr< FastFsFile > m_pFile;
};

TEST_F( FastFsFileTest, FileOpened_ReadFromFile_ReturnsNoData ) {
    EXPECT_THAT( m_pFile->Read().size(), Eq( 0 ) );
}

TEST_F( FastFsFileTest, FileOpened_SizeReturnsZero ) {
    EXPECT_THAT( m_pFile->Size(), Eq( 0 ) );
}

TEST_F( FastFsFileTest, FileOpened_WriteToFille_SizeIsCorrect ) {
    std::vector< uint8_t > data { 0, 1, 2, 3 };
    m_pFile->Write( data );

    EXPECT_THAT( m_pFile->Size(), Eq( data.size() ) );
}

TEST_F( FastFsFileTest, FileOpened_WriteToFille_ReadReturnsWrittenData ) {
    std::vector< uint8_t > data { 0, 1, 2, 3 };
    m_pFile->Write( data );

    EXPECT_THAT( m_pFile->Read(), ContainerEq( data ) );
}

class FastFsFileModeTest : public ::testing::Test {
protected:
    void SetUp() {
        m_pFastFs.reset( new FastFs() );
        m_pDir = m_pFastFs->CreateDirectory( "test" );
    }

    void TearDown() {
    }

    std::unique_ptr< FastFs > m_pFastFs;
    std::shared_ptr< FastFsDirectory > m_pDir;
    std::shared_ptr< FastFsFile > m_pFile;
};

TEST_F( FastFsFileModeTest, FileOpenedInOverWriteModeWithWrittenData_WriteAgain_OverwritesOldData ) {
    m_pFile = m_pDir->OpenFile( "testfile", FileOpenMode::OVERWRITE );
    std::vector< uint8_t > data { 0, 1, 2, 3 };
    m_pFile->Write( data );

    std::vector< uint8_t > newData { 4, 5, 6, 7, 8 };
    m_pFile->Write( newData );

    EXPECT_THAT( m_pFile->Read(), ElementsAre( 4, 5, 6, 7, 8 ) );
}

TEST_F( FastFsFileModeTest, FileOpenedInAppendModeWithWrittenData_WriteAgain_AppendsToOldOldData ) {
    m_pFile = m_pDir->OpenFile( "testfile", FileOpenMode::APPEND );
    std::vector< uint8_t > data { 0, 1, 2, 3 };
    m_pFile->Write( data );

    std::vector< uint8_t > newData { 4, 5, 6, 7, 8 };
    m_pFile->Write( newData );

    EXPECT_THAT( m_pFile->Read(), ElementsAre( 0, 1, 2, 3, 4, 5, 6, 7, 8 ) );
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
