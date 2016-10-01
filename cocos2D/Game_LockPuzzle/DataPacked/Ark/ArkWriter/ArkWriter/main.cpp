//
// main.cpp
// Liam de Koster-Kjaer
// February 2013
//

// Includes
#include <string>
#include <cassert>
#include <windows.h>
#include <algorithm>
#include "RKHeap.h"
#include "FileInfoLib.h"
#include "FileSystem/ArkWriter.h"

// Defines
typedef bool(ArkWriterFunc)();

// Forward declarations
void  ParseArguments(int argc, char** argv);
bool  CreateSingleArk(std::string& sArkList, std::string& sDataDir, std::string& sOutDir, std::string& sArkFilename, std::string& sExcludeList, std::string& sExternList, std::string& sExtractList, std::string& sEncryptList, std::string& sNoCompressList);

// Command line arguments
namespace Arg
{
  enum Type
  {
    INVALID_ARG = -1, 
    FUNCTION, 
    DATADIR, 
    OUTDIR, 
    ARKLISTDIR, 
    ARKLIST, 
    EXCLUDELIST,
    EXTERNLIST, 
    EXTRACTLIST, 
    ENCRYPTLIST, 
    NOCOMPLIST, 
    FILENAME, 
    PLATFORM, 
    SUFFIX, 
    ALLOWDUPES,
    COUNT
  };
  const char* AsString[COUNT] = 
  {
    "function",       // Declared in Func namespace
    "datadir",        // Full filepath
    "outdir",         // Full filepath
    "arklistdir",     // Full filepath
    "arklist",        // Full filepath
    "excludelist",    // Full filepath
    "externlist",     // Full filepath
    "extractlist",    // Full filepath
    "encryptlist",    // Full filepath
    "nocompresslist", // Full filepath
    "filename",       // Filename only, no path
    "platform",       // Used to generate ArkFilenames, legacy from old DLCTool
    "suffix",         // Used to generate ArkFilenames, legacy from old DLCTool
    "allowdupes"      // Will allow a build when duplicate files are found
  };
};

// Functions
namespace Func
{
  bool CreateArkList();
  bool CreateArk();
  bool CreateArks();

  enum Type
  {
    INVALID_FUNC = -1, 
    CREATE_ARKLIST, 
    CREATE_ARK, 
    CREATE_ARKS, 
    COUNT
  };
  const char* AsString[COUNT] = 
  {
    "create_arklist", 
    "create_ark", 
    "create_arks"
  };
  ArkWriterFunc* Function[COUNT] = 
  {
    CreateArkList, 
    CreateArk, 
    CreateArks
  };
};

// Globals
std::string gArg[Arg::COUNT];
Func::Type gFunction = Func::INVALID_FUNC;

//!>> --------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
  printf("\n\n ---------- ArkWriterTool - ArkVersion %s ---------- \n\n", g_kArkVersion);

  RKHeap_InitModule();

  ParseArguments(argc, argv);

  int iReturn = Func::Function[gFunction]() ? EXIT_SUCCESS : EXIT_FAILURE;

  AllocatedObjectManager::CleanUp();

  return iReturn;
}

void ParseArguments(int argc, char** argv)
{
  if (argc != 0)
  {
    // Clear args
    for (int i = 0; i < Arg::COUNT; ++i)
      gArg[i] = "";

    char* pKey = 0;
    for (int i = 1; i < argc; ++i) // Skip first argument(program path)
    {
      pKey = argv[i];
      if (*pKey == '-')
      {
        ++pKey;
        for (int j = 0; j < Arg::COUNT; ++j)
        {
          if (0==strcmp(pKey, Arg::AsString[j]))
          {
            gArg[j] = argv[++i];
            assert(i < argc);
            break;
          }
        }
      }
      else
        assert(!"ParseArguments - Error! Should not have hit this point\n");
    }
    // Set function
    for (unsigned int i = 0; i < Func::COUNT; ++i)
    {
      if (gArg[Arg::FUNCTION] == Func::AsString[i])
      {
        gFunction = static_cast<Func::Type>(i);
        break;
      }
    }
    // Safety checks
    if (!gArg[Arg::DATADIR].empty())
      assert(CheckValidPath(gArg[Arg::DATADIR])       && "ParseArguments - Data directory is invalid");
    if (!gArg[Arg::OUTDIR].empty())
      assert(CheckValidPath(gArg[Arg::OUTDIR], true)  && "ParseArguments - Output directory is invalid");
    if (!gArg[Arg::ARKLISTDIR].empty())
      assert(CheckValidPath(gArg[Arg::ARKLISTDIR])    && "ParseArguments - Arklist directory is invalid");
  }
  else
    printf("ParseArguments - Zero arguments!\n");
}

bool CreateSingleArk(std::string& sDataDir, std::string& sOutDir, std::string& sArkFilename, 
                     FileList& ArkList, FileList& ExternList, FileList& ExtractList, FileList& EncryptList, FileList& NoCompressList)
{
  bool bSuccess = true;

  // Create OutDir if necessary
  if (!DirExists(sOutDir.c_str()))
    CreateDirectory(sOutDir.c_str(), NULL);
  assert(CheckValidPath(sOutDir) && "CreateArk - Output directory path is invalid");

  std::string sFullFileName = "";

  // Handle ark external files, remove from arklist and copy files to outdir
  std::string sCommand = "";
  for (unsigned int i = 0, size = ExternList.size(); i < size; ++i)
  {
    for (FileListIter it = ArkList.begin(), itEnd = ArkList.end(); it != itEnd; ++it)
    {
      if (ExternList[i]->sSubDir == (*it)->sSubDir &&
          ExternList[i]->sFileName == (*it)->sFileName &&
          ExternList[i]->sExtension == (*it)->sExtension)
      {
        // Copy ExternList files to OutDir
        (*it)->FullFileName(sFullFileName, sDataDir);
        sCommand = "xcopy /q /i /y \"" + sFullFileName + "\" \"" + gArg[Arg::OUTDIR] + (*it)->sSubDir + "\"";
        system(sCommand.c_str());
        // Remove ExternList files from ArkList
        ArkList.erase(it);
        break;
      }
    }
  }

  // ArkList could have no files left after external files are removed
  if (ArkList.size())
  {
    // Initialise ArkWriter
    ArkWriter aw;
    if (aw.InitialiseArk(sArkFilename.c_str()))
    {
      printf("CreateSingleArk - Beginning to write archive: %s\n", sArkFilename.c_str());

      FileInfo* pFile     = 0;
      std::string sSubDir = "";
      bool bExtract       = false;
      bool bCompress      = true;
      bool bEncrypt       = false;

      // Add ArkList files to Ark
      for (unsigned int i = 0, size = ArkList.size(); i < size; ++i)
      {
        pFile     = ArkList[i];
        bExtract  = ExtractList.GetFile(pFile->sFileName, pFile->sExtension, pFile->sSubDir) ? true : false;
        bEncrypt  = EncryptList.GetFile(pFile->sFileName, pFile->sExtension, pFile->sSubDir) ? true : false;
        bCompress = NoCompressList.GetFile(pFile->sFileName, pFile->sExtension, pFile->sSubDir) ? false : true;
        pFile->FullFileName(sFullFileName, gArg[Arg::DATADIR]);
        sSubDir   = pFile->sSubDir;

        // Use forward slashes for subdir paths
        for (unsigned int j = 0, s = sSubDir.size(); j < s; ++j)
        {
          if (sSubDir[j] == '\\')
            sSubDir[j] = '/';
        }

        if (aw.AddFileToArk(sFullFileName.c_str(), sSubDir.c_str(), bExtract, bCompress, bEncrypt))
          printf("CreateSingleArk - Successfully added file to archive: %s\n", sFullFileName.c_str());
        else
        {
          printf("CreateSingleArk - Critical Error! Failed to write file to archive: %s\nFailed to write archive\n", sFullFileName.c_str());
          bSuccess = false;
        }
      }

      // Finalise Ark
      if (aw.FinaliseArk())
        printf("CreateSingleArk - Success! Finished writing archive\n");
      else
      {
        printf("CreateSingleArk - Critical Error! Failed to finalise archive\n");
        bSuccess = false;
      }
    }
    else
    {
      printf("ArkWriter::CreateArk - Critical Error! Failed to initialise archive\n");
      bSuccess = false;
    }
  }
  else
  {
    printf("ArkWriter::CreateArk - Arklist contains no files, Ark archive was not written. Either this arklist contained only ark-external files or an error has occurred\n");
  }

  return bSuccess;
}

/**
* CreateArkList
* -function create_arklist -datadir -outdir -filename -excludelist(optional)
**/
bool Func::CreateArkList()
{
  printf("//!>> ArkWriter::CreateArkList\n");

  assert(!gArg[Arg::DATADIR].empty()        && "CreateArkList - Invalid trunk directory argument");
  assert(!gArg[Arg::OUTDIR].empty()         && "CreateArkList - Invalid output directory argument");
  assert(!gArg[Arg::FILENAME].empty()       && "CreateArkList - Invalid Ark filename");
  assert(CheckValidPath(gArg[Arg::DATADIR]) && "CreateArkList - Data directory path is invalid");

  // Create OutDir if necessary
  if (!DirExists(gArg[Arg::OUTDIR].c_str()))
    CreateDirectory(gArg[Arg::OUTDIR].c_str(), NULL);
  assert(CheckValidPath(gArg[Arg::OUTDIR])  && "CreateArkList - Output directory path is invalid");

  // Create filelist of data dir files
  FileList ArkList(gArg[Arg::DATADIR], "*.*|!.ark", true);
  RefreshList(ArkList, gArg[Arg::DATADIR]);
  // Exclude exclude list files
  FileList ExcludeList(gArg[Arg::EXCLUDELIST]);
  RefreshList(ExcludeList, gArg[Arg::DATADIR]);
  ExcludeFiles(ExcludeList, ArkList);
  // Write arklist
  std::string sFilename = gArg[Arg::OUTDIR] + gArg[Arg::FILENAME];
  return ArkList.WriteFileList(sFilename);
}

/**
* CreateArk
* -function create_ark -datadir -outdir -arklist -filename -externlist(optional) -extractlist(optional) -encryptist(optional) -nocompresslist(optional)
**/
bool Func::CreateArk()
{
  printf("//!>> ArkWriter::CreateArk\n");

  assert(!gArg[Arg::DATADIR].empty()        && "CreateArk - Invalid trunk directory argument");
  assert(!gArg[Arg::OUTDIR].empty()         && "CreateArk - Invalid output directory argument");
  assert(!gArg[Arg::ARKLIST].empty()        && "CreateArk - Invalid ArkList argument");
  assert(!gArg[Arg::FILENAME].empty()       && "CreateArk - Invalid Ark filename");
  assert(CheckValidPath(gArg[Arg::DATADIR]) && "CreateArk - Data directory path is invalid");

  FileList ArkList(gArg[Arg::ARKLIST]);
  FileList ExternList(gArg[Arg::EXTERNLIST]);
  FileList ExtractList(gArg[Arg::EXTRACTLIST]);
  FileList EncryptList(gArg[Arg::ENCRYPTLIST]);
  FileList NoCompressList(gArg[Arg::NOCOMPLIST]);

  // Assure there are no duplicate files in the arklist
  bool bDupes = false;
  for (unsigned int i = 0, size = ArkList.size(); i < size; ++i)
  {
    for (unsigned int j = i+1; j < size; ++j)
    {
      if (ArkList[i]->sFileName  == ArkList[j]->sFileName &&
          ArkList[i]->sExtension == ArkList[j]->sExtension)
      {
        if (ExternList.GetFile(ArkList[i]->sFileName, ArkList[i]->sExtension))
        {// File is in extern list
          //printf("WARNING: File in externlist has a file with same name and extension\n");
          //printf("Duplicate files: %s%s.%s and %s%s.%s\n", ArkList[i]->sSubDir.c_str(), ArkList[i]->sFileName.c_str(), ArkList[i]->sExtension.c_str(), ArkList[j]->sSubDir.c_str(), ArkList[j]->sFileName.c_str(), ArkList[j]->sExtension.c_str());
          //printf("Found in arklist: %s\n", gArg[Arg::ARKLIST].c_str());
        }
        else
        {
          printf("Duplicate files: %s%s.%s and %s%s.%s\n", ArkList[i]->sSubDir.c_str(), ArkList[i]->sFileName.c_str(), ArkList[i]->sExtension.c_str(), ArkList[j]->sSubDir.c_str(), ArkList[j]->sFileName.c_str(), ArkList[j]->sExtension.c_str());
          printf("Found in arklist: %s\n", gArg[Arg::ARKLIST].c_str());
          bDupes = true;
        }
      }
    }
  }

  if (bDupes && (gArg[Arg::ALLOWDUPES] != "true" && gArg[Arg::ALLOWDUPES] != "True"))
  {
    printf("Ark will not be written\n");
    system("pause");
    return false;
  }

  // Create OutDir if necessary
  if (!DirExists(gArg[Arg::OUTDIR].c_str()))
    CreateDirectory(gArg[Arg::OUTDIR].c_str(), NULL);
  assert(CheckValidPath(gArg[Arg::OUTDIR])  && "CreateArk - Output directory path is invalid");

  std::string sArkFilename = gArg[Arg::OUTDIR] + gArg[Arg::FILENAME];
  return CreateSingleArk(gArg[Arg::DATADIR], gArg[Arg::OUTDIR], sArkFilename, ArkList, ExternList, ExtractList, EncryptList, NoCompressList);
}

/**
* CreateArks
* -function create_arks -arklistdir -datadir -outdir -platform -suffix -externlist(optional) -extractlist(optional) -encryptist(optional) -nocompresslist(optional)
**/
bool Func::CreateArks()
{
  printf("//!>> ArkWriter::CreateArks\n");

  assert(!gArg[Arg::ARKLISTDIR].empty()     && "CreateArks - Invalid arklist directory argument");
  assert(!gArg[Arg::DATADIR].empty()        && "CreateArks - Invalid trunk directory argument");
  assert(!gArg[Arg::OUTDIR].empty()         && "CreateArks - Invalid output directory argument");
  assert(CheckValidPath(gArg[Arg::DATADIR]) && "CreateArks - Data directory path is invalid");

  bool bSuccess = true;

  // Create OutDir if necessary
  if (!DirExists(gArg[Arg::OUTDIR].c_str()))
    CreateDirectory(gArg[Arg::OUTDIR].c_str(), NULL);
  assert(CheckValidPath(gArg[Arg::OUTDIR])  && "CreateArks - Output directory path is invalid");

  // Load arklists
  FileList ArkListFiles(gArg[Arg::ARKLISTDIR], "*.arklist", false);
  unsigned int iNumArkLists = ArkListFiles.size();
  FileGroup ArkLists;
  std::string sArkListFilename = "";
  for (unsigned int i = 0; i < iNumArkLists; ++i)
  {
    ArkListFiles[i]->FullFileName(sArkListFilename, gArg[Arg::ARKLISTDIR]);
    ArkLists.push_back(new FileList(sArkListFilename));
  }
  FileList ExternList(gArg[Arg::EXTERNLIST]);
  FileList ExtractList(gArg[Arg::EXTRACTLIST]);
  FileList EncryptList(gArg[Arg::ENCRYPTLIST]);
  FileList NoCompressList(gArg[Arg::NOCOMPLIST]);

  // Make one big list of all files - easier to check for duplicates.
  FileList* pAllFilesList = new FileList();
  for (unsigned int i = 0; i < iNumArkLists; ++i)
  {
    for (unsigned int j = 0, size = ArkLists[i]->size(); j < size; ++j)
    {
      FileInfo* pInfo = new FileInfo();
      *pInfo = (*(*ArkLists[i])[j]);  // OMG EWW
      pAllFilesList->push_back(pInfo);
    }
  }

  // Check for multiple files with the same name
  bool bDupes = false;
  for (unsigned int i = 0, size = pAllFilesList->size(); i < size; ++i)
  {
    for (unsigned int j = i+1; j < size; ++j)
    {
      if ((*pAllFilesList)[i]->sFileName  == (*pAllFilesList)[j]->sFileName &&
          (*pAllFilesList)[i]->sExtension == (*pAllFilesList)[j]->sExtension)
      {
        if (ExternList.GetFile((*pAllFilesList)[i]->sFileName, (*pAllFilesList)[i]->sExtension))
        {// File is in extern list
          printf("WARNING: File in externlist has a file with same name and extension\n");
          printf("Duplicate files: %s%s.%s and %s%s.%s\n", (*pAllFilesList)[i]->sSubDir.c_str(), (*pAllFilesList)[i]->sFileName.c_str(), (*pAllFilesList)[i]->sExtension.c_str(), (*pAllFilesList)[j]->sSubDir.c_str(), (*pAllFilesList)[j]->sFileName.c_str(), (*pAllFilesList)[j]->sExtension.c_str());
        }
        else
        {
          printf("Duplicate files: %s%s.%s and %s%s.%s\n", (*pAllFilesList)[i]->sSubDir.c_str(), (*pAllFilesList)[i]->sFileName.c_str(), (*pAllFilesList)[i]->sExtension.c_str(), (*pAllFilesList)[j]->sSubDir.c_str(), (*pAllFilesList)[j]->sFileName.c_str(), (*pAllFilesList)[j]->sExtension.c_str());
          bDupes = true;
        }
      }
    }
  }

  delete pAllFilesList;
  pAllFilesList = 0;

  if (bDupes && (gArg[Arg::ALLOWDUPES] != "true" && gArg[Arg::ALLOWDUPES] != "True"))
  {
    printf("Ark will not be written\n");
    system("pause");
    return false;
  }
  
  std::string sArkFilename;
  for (unsigned int i = 0, size = ArkLists.size(); i < size; ++i)
  {
    sArkFilename = gArg[Arg::OUTDIR];
    sArkFilename += ArkListFiles[i]->sFileName;
    if (!gArg[Arg::PLATFORM].empty())
      sArkFilename += "_" + gArg[Arg::PLATFORM];
    if (!gArg[Arg::SUFFIX].empty())
      sArkFilename += "_" + gArg[Arg::SUFFIX];
    sArkFilename += ".ark";
    if (!CreateSingleArk(gArg[Arg::DATADIR], gArg[Arg::OUTDIR], sArkFilename, *ArkLists[i], ExternList, ExtractList, EncryptList, NoCompressList))
      bSuccess = false;
  }

  return bSuccess;
}

#pragma region CUSTOM_ALLOCATION_JUNK
// These are needed by GLF through CasualCore 
#include "glwebtools/internal/glwebtools_memory.h"
namespace glf
{
  class App;
  enum MemHint;
}

glf::App* NewApp(int argc, char** argv)
{
  return NULL;
}

void* GlfAlloc(size_t size)
{
  //return RKHeap_Alloc(size);
  return malloc(size);
}

void* GlfAlloc(size_t size, int)
{
  //return RKHeap_Alloc(size);
  return malloc(size);
}

void* GlfAlloc(size_t size, glf::MemHint)
{
  //return RKHeap_Alloc(size);
  return malloc(size);
}

void GlfFree(void* ptr)
{
  //RKHeap_Free(ptr);
  free(ptr);
}

void* Glwt2Alloc(std::size_t size, glwebtools::MemHint memhint, const char* filename, const char* function, int line)
{
  //return RKHeap_Alloc(size);
  return malloc(size);
}

void* Glwt2Realloc(void* ptr, std::size_t size, glwebtools::MemHint memhint, const char* filename, const char* function, int line)
{
  //return RKHeap_Realloc(ptr,size);
  return realloc(ptr, size);
}

void Glwt2Free(void* ptr)
{
  //if (ptr) RKHeap_Free(ptr);
  free(ptr);
}

//void* GlwtAlloc(std::size_t size, glwt::MemHint memhint, const char* filename, const char* function, int line)
//{
//	return RKHeap_Alloc(size);
//}
//
//void* GlwtRealloc(void* ptr, std::size_t size, glwt::MemHint memhint, const char* filename, const char* function, int line)
//{
//	return RKHeap_Realloc(ptr,size);
//}

void GlwtFree(void* ptr)
{
  //if (ptr) RKHeap_Free(ptr);
  free(ptr);
}
#pragma endregion
