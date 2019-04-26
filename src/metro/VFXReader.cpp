#include "VFXReader.h"
#include "MetroCompression.h"

#include <fstream>

static const CharString sGameVersions[] = {
    "Unknown",
    "Redux (2033 / Last Light)",
    "Arktika.1",
    "Exodus"
};

VFXReader::VFXReader() {

}

VFXReader::~VFXReader() {

}

bool VFXReader::LoadFromFile(const fs::path& filePath) {
    bool result = false;

    this->Close();

    LogPrint(LogLevel::Info, "loading vfx file...");

    std::ifstream file(filePath, std::ifstream::binary);
    if (file.good()) {
        BytesArray fileData;

        file.seekg(0, std::ios::end);
        fileData.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(rcast<char*>(fileData.data()), fileData.size());
        file.close();

        MemStream stream(fileData.data(), fileData.size());

        mVersion = stream.ReadTyped<uint32_t>();
        mCompressionType = stream.ReadTyped<uint32_t>();

        LogPrint(LogLevel::Info, "vfx version = " + std::to_string(mVersion) + ", compression = " + std::to_string(mCompressionType));

        if ((mVersion > kVFXVersionUnknown && mVersion < kVFXVersionMax) && mCompressionType == MetroCompression::Type_LZ4) {
            if (mVersion >= kVFXVersionExodus) {
                mContentVersion = stream.ReadStringZ();
            }
            stream.ReadStruct(mGUID); // guid, seems to be static across the game
            const size_t numVFS = stream.ReadTyped<uint32_t>();
            const size_t numFiles = stream.ReadTyped<uint32_t>();
            const size_t numDuplicates = stream.ReadTyped<uint32_t>();

            LogPrint(LogLevel::Info, "game version = " + sGameVersions[mVersion]);
            LogPrint(LogLevel::Info, "vfx content version = " + mContentVersion);
            LogPrintF(LogLevel::Info, "vfx guid = %08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x",
                mGUID.a, mGUID.b, mGUID.c, mGUID.d, mGUID.e[0], mGUID.e[1], mGUID.e[2], mGUID.e[3], mGUID.e[4], mGUID.e[5]);

            LogPrintF(LogLevel::Info, "packages = %lld, files = %lld, duplicates = %lld", numVFS, numFiles, numDuplicates);

            mPaks.resize(numVFS);
            for (Package& pak : mPaks) {
                pak.name = stream.ReadStringZ();

                const uint32_t numStrings = stream.ReadTyped<uint32_t>();
                pak.levels.resize(numStrings);
                for (auto& s : pak.levels) {
                    s = stream.ReadStringZ();
                }

                pak.chunk = stream.ReadTyped<uint32_t>();
            }

            mFiles.resize(numFiles);
            size_t fileIdx = 0;
            for (MetroFile& mf : mFiles) {
                mf.idx = fileIdx;

                this->ReadFileDescription(mf, stream);

                if (!mf.IsFile()) {
                    mFolders.push_back(fileIdx);
                }

                ++fileIdx;
            }

            mDuplicates.resize(numDuplicates);
            size_t duplicateIdx = 0;
            for (MetroFile& mf : mDuplicates) {
                this->ReadFileDescription(mf, stream, true);

                MetroFile& baseMf = mFiles[mf.baseIdx];
                mf.duplicates = baseMf.duplicates;
                baseMf.duplicates = duplicateIdx;

                mf.name = baseMf.name;

                ++duplicateIdx;
            }

            mBasePath = filePath.parent_path();
            mFileName = filePath.filename().string();
            result = true;

            LogPrint(LogLevel::Info, "vfx loaded successfully");
        } else {
            LogPrint(LogLevel::Error, "unknown version or compression");
        }
    } else {
        LogPrint(LogLevel::Error, "failed to open file");
    }

    return result;
}

void VFXReader::Close() {
    mPaks.resize(0);
    mFiles.resize(0);
    mFolders.resize(0);
    mDuplicates.resize(0);
}

const CharString& VFXReader::GetContentVersion() const {
    return mContentVersion;
}

size_t VFXReader::GetVersion() const {
    return mVersion;
}

MemStream VFXReader::ExtractFile(const size_t fileIdx, const size_t subOffset, const size_t subLength) const {
    MemStream result;

    const MetroFile& mf = mFiles[fileIdx];
    const Package& pak = mPaks[mf.pakIdx];

    fs::path pakPath = mBasePath / pak.name;
    std::ifstream file(pakPath, std::ifstream::binary);
    if (file.good()) {
        file.seekg(mf.offset);

        const size_t streamOffset = (subOffset == kInvalidValue) ? 0 : std::min<size_t>(subOffset, mf.sizeUncompressed);
        const size_t streamLength = (subLength == kInvalidValue) ? (mf.sizeUncompressed - streamOffset) : (std::min<size_t>(subLength, mf.sizeUncompressed - streamOffset));

        uint8_t* fileContent = rcast<uint8_t*>(malloc(mf.sizeCompressed));
        file.read(rcast<char*>(fileContent), mf.sizeCompressed);

        if (mf.sizeCompressed == mf.sizeUncompressed) {
            result = MemStream(fileContent, mf.sizeUncompressed, true);
        } else {
            uint8_t* uncompressedContent = rcast<uint8_t*>(malloc(mf.sizeUncompressed));
            const size_t decompressResult = MetroCompression::DecompressStream(fileContent, mf.sizeCompressed, uncompressedContent, mf.sizeUncompressed);
            if (decompressResult == mf.sizeUncompressed) {
                result = MemStream(uncompressedContent, mf.sizeUncompressed, true);
            }

            free(fileContent);
        }

        if (result.Good()) {
            result.SetWindow(streamOffset, streamLength);
        }
    }

    return std::move(result);
}

bool VFXReader::Good() const {
    return !mFiles.empty();
}

const CharString& VFXReader::GetSelfName() const {
    return mFileName;
}

const MyArray<Package>& VFXReader::GetAllPacks() const {
    return mPaks;
}

const MyArray<MetroFile>& VFXReader::GetFiles() const {
    return mFiles;
}

const MyArray<size_t>& VFXReader::GetAllFolders() const {
    return mFolders;
}

const MetroFile* VFXReader::GetFolder(const CharString& folderPath, const MetroFile* inFolder) const {
    CharString::size_type slashPos = folderPath.find_first_of('\\'), lastSlashPos = 0;
    const MetroFile* folder = (inFolder == nullptr) ? &mFiles.front() : inFolder;
    while (slashPos != CharString::npos) {
        size_t folderIdx = MetroFile::InvalidFileIdx;

        CharString name = folderPath.substr(lastSlashPos, slashPos - lastSlashPos);
        for (const size_t idx : *folder) {
            const MetroFile& mf = mFiles[idx];
            if (name == mf.name) {
                folderIdx = idx;
                break;
            }
        }

        if (folderIdx == MetroFile::InvalidFileIdx) { // failed to find
            return nullptr;
        }

        lastSlashPos = slashPos + 1;
        slashPos = folderPath.find_first_of('\\', lastSlashPos);
        folder = &mFiles[folderIdx];
    }

    return folder;
}

size_t VFXReader::FindFile(const CharString& fileName, const MetroFile* inFolder) const {
    size_t result = MetroFile::InvalidFileIdx;

    const MetroFile* folder = (inFolder == nullptr) ? &mFiles.front() : inFolder;

    CharString::size_type lastSlashPos = fileName.find_last_of('\\');
    if (lastSlashPos != CharString::npos) {
        lastSlashPos++;
        folder = this->GetFolder(fileName.substr(0, lastSlashPos), inFolder);
    } else {
        lastSlashPos = 0;
    }

    if (folder) {
        CharString name = fileName.substr(lastSlashPos);

        for (const size_t idx : *folder) {
            const MetroFile& mf = mFiles[idx];
            if (name == mf.name) {
                result = idx;
                break;
            }
        }
    }

    return result;
}

const MetroFile& VFXReader::GetRootFolder() const {
    return mFiles.front();
}

const MetroFile* VFXReader::GetParentFolder(const size_t fileIdx) const {
    const MetroFile* result = nullptr;

    for (const size_t idx : mFolders) {
        const MetroFile* folder = &mFiles[idx];
        if (folder->ContainsFile(fileIdx)) {
            result = folder;
            break;
        }
    }

    return result;
}

const MetroFile& VFXReader::GetFile(const size_t idx) const {
    return mFiles[idx];
}

size_t VFXReader::CountFilesInFolder(const size_t idx) const {
    size_t result = 0;

    const MetroFile& folder = mFiles[idx];
    for (const size_t idx : folder) {
        const MetroFile& mf = mFiles[idx];
        if (mf.IsFile()) {
            result++;
        } else {
            result += this->CountFilesInFolder(idx);
        }
    }

    return result;
}

MyArray<size_t> VFXReader::FindFilesInFolder(const size_t folderIdx, const CharString& extension, const bool withSubfolders) {
    MyArray<size_t> result;

    const MetroFile& folder = mFiles[folderIdx];
    if (!folder.IsFile()) {
        for (const size_t idx : folder) {
            const MetroFile& mf = mFiles[idx];

            if (!mf.IsFile()) {
                const MyArray<size_t>& v = this->FindFilesInFolder(mf.idx, extension, withSubfolders);
                result.insert(result.end(), v.begin(), v.end());
            } else {
                if (StrEndsWith(mf.name, extension)) {
                    result.push_back(mf.idx);
                }
            }
        }
    }

    return std::move(result);
}

MyArray<size_t> VFXReader::FindFilesInFolder(const CharString& folder, const CharString& extension, const bool withSubfolders) {
    MyArray<size_t> result;

    const MetroFile* folderPtr = this->GetFolder(folder);
    if (folderPtr) {
        result = this->FindFilesInFolder(folderPtr->idx, extension, withSubfolders);
    }

    return std::move(result);
}

static CharString ReadEncryptedFileName(MemStream& stream) {
    CharString result;

    const uint16_t stringHeader = stream.ReadTyped<uint16_t>();
    const size_t stringLen = (stringHeader & 0xFF);
    const char xorMask = scast<char>((stringHeader >> 8) & 0xFF);

    result.reserve(stringLen);
    for (size_t i = 1; i < stringLen; ++i) {
        const char ch = stream.ReadTyped<char>();
        result.push_back(ch ^ xorMask);
    }

    stream.ReadTyped<char>(); // terminating null

    return result;
};

void VFXReader::ReadFileDescription(MetroFile& mf, MemStream& stream, const bool isDuplicate) {
    mf.flags = stream.ReadTyped<uint16_t>();

    if (mf.IsFile()) {
        mf.pakIdx = stream.ReadTyped<uint16_t>();
        mf.offset = stream.ReadTyped<uint32_t>();
        mf.sizeUncompressed = stream.ReadTyped<uint32_t>();
        mf.sizeCompressed = stream.ReadTyped<uint32_t>();

        if (isDuplicate) {
            mf.baseIdx = stream.ReadTyped<uint32_t>();
        }

        mf.duplicates = kInvalidValue;
    } else {
        mf.numFiles = stream.ReadTyped<uint16_t>();
        mf.firstFile = stream.ReadTyped<uint32_t>();
    }

    if (!isDuplicate) {
        mf.name = ReadEncryptedFileName(stream);
    }
}
