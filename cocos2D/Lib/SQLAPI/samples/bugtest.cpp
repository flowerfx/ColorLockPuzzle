#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include <SQLAPI.h>
#include <samisc.h>

#include <string>
#include <iostream>

static FILE *pFile = NULL;
size_t nTotalBound;
size_t FromFileWriter(SAPieceType_t &ePieceType,
    void *pBuf, size_t nLen, void *pAddlData)
{
    if (ePieceType == SA_FirstPiece)
    {
        const char *sFilename = (const char *)pAddlData;
        pFile = fopen(sFilename, "rb");
        if (!pFile)
            SAException::throwUserException(-1, _TSA("Can not open file '%s'"),
            (const SAChar*)SAString(sFilename));
        nTotalBound = 0;
    }

    size_t nRead = fread(pBuf, 1, nLen, pFile);
    nTotalBound += nRead;

    // show progress
    printf("%d bytes of file bound\n", nTotalBound);

    if (feof(pFile))
    {
        if (ePieceType == SA_FirstPiece)
            ePieceType = SA_OnePiece;
        else
            ePieceType = SA_LastPiece;

        fclose(pFile);
        pFile = NULL;
    }
    return nRead;
}

SAString ReadWholeFile(const char *sFilename)
{
    SAString s;
    char sBuf[1024];
    FILE *pFile = fopen(sFilename, "rb");

    if (!pFile)
        SAException::throwUserException(-1, _TSA("Error opening file '%s'\n"),
        (const SAChar*)SAString(sFilename));

    do
    {
        size_t nRead = fread(sBuf, 1, sizeof(sBuf), pFile);
        s += SAString((const void*)sBuf, nRead);
    } while (!feof(pFile));

    fclose(pFile);
    return s;
}

SAString ReadWholeTextFile(const SAChar *szFilename)
{
    SAString s;
    char szBuf[32 * 1024];
    FILE *pFile = _tfopen(szFilename, _TSA("rb"));

    if (!pFile)
        SAException::throwUserException(-1, _TSA("Error opening file '%s'\n"),
        (const SAChar*)SAString(szFilename));

    do
    {
        size_t nRead = fread(szBuf, 1, sizeof(szBuf), pFile);
        s += SAString(szBuf, nRead);
    } while (!feof(pFile));

    fclose(pFile);
    return s;
}

void WriteWholeFile(const char *sFilename, const SAString& data)
{
    FILE *pFile = fopen(sFilename, "wb");
    size_t n, written = 0, len = data.GetBinaryLength();
    const void* pData = (const void*)data;

    sa_printf(_TSA("PRGLEN: %d\n"), len);

    if (!pFile)
        SAException::throwUserException(-1, _TSA("Error opening file '%s'\n"),
        (const SAChar*)SAString(sFilename));

    while (len > written) {
        n = fwrite((const char*)pData + written, 1, sa_min(1024, len - written), pFile);
        if (n <= 0)
            break;
        written += n;
    }

    fclose(pFile);
}

size_t nTotalRead;

void IntoFileReader(
    SAPieceType_t ePieceType,
    void *pBuf,
    size_t nLen,
    size_t nBlobSize,
    void *pAddlData)
{
    const char *sFilename = (const char *)pAddlData;

    if (ePieceType == SA_FirstPiece || ePieceType == SA_OnePiece)
    {
        nTotalRead = 0;

        pFile = fopen(sFilename, "wb");
        if (!pFile)
            SAException::throwUserException(-1, _TSA("Can not open file '%s' for writing"),
            (const SAChar*)SAString(sFilename));
    }

    fwrite(pBuf, 1, nLen, pFile);

    nTotalRead += nLen;

    if (ePieceType == SA_LastPiece || ePieceType == SA_OnePiece)
    {
        fclose(pFile);
        pFile = NULL;
        printf("%s : %d bytes of %d read\n",
            sFilename, nTotalRead, nBlobSize);
    }
}

//#include "Scrollable_Cursor.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include <SQLAPI.h>
#include <samisc.h>

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    SAConnection con;

    try
    {
        //con.setOption("UseAPI") = "OCI7";
        //con.setOption(_TSA("OCI_ATTR_RECEIVE_TIMEOUT")) = _TSA("2000");
        //con.setOption(_TSA("OCI_ATTR_SEND_TIMEOUT")) = _TSA("2000");
        //con.Connect("ora111", "sys", "java", SA_Oracle_Client);
        //con.setOption("UseAPI") = "DB-Library";
        //con.setOption("SSPROP_INIT_MARSCONNECTION") = "VARIANT_FALSE";
        //con.Connect( "BEDLAM-M\\EN2008R2@master", "sa", "xxx", SA_SQLServer_Client);
        //con.Connect(_TSA("BEDLAM-M\\EN2014@test"), _TSA(""), _TSA(""), SA_SQLServer_Client);
        //con.Connect(_TSA("BEDLAM-M\\EN2012@test"), _TSA(""), _TSA(""), SA_SQLServer_Client);
        //con.Connect(_TSA("BEDLAM-M\\EN2005@test"), _TSA(""), _TSA(""), SA_SQLServer_Client);
        //con.Connect(_TSA("BEDLAM-M\\EN2008R2@test"), _TSA(""), _TSA(""), SA_SQLServer_Client);
        //con.Connect( "LocalServer", "sa", "java", SA_ODBC_Client);
        //con.Connect("TestAccess", "", "", SA_ODBC_Client);
        //con.Connect( "test", "postgres", "java", SA_PostgreSQL_Client);
        //con.Connect(_TSA("test"), _TSA("root"), _TSA(""), SA_MySQL_Client);

        //con.Connect("d:\\test.db", "", "", SA_SQLite_Client);

        con.setOption("SQLNCLI.LIBS") = "SQLSRV32.DLL";
        //con.setOption("SQLNCLI.LIBS") = "SQLNCLI11.DLL";
        con.Connect(_TSA("BEDLAM-M\\EN2014@test"), _TSA(""), _TSA(""), SA_SQLServer_Client);
        //con.Connect("bedlam-m.bedlam,1433@master", "", "", SA_SQLServer_Client);

        //con.setAutoCommit(SA_AutoCommitOff);

        SACommand cmd(&con, _TSA("update t1 set f6=:1 where f1=1"));
        cmd.Param(1).setAsLongBinary() = _TSA("xxx");
        cmd.Execute();

        while (cmd.isResultSet())
        {
            while (cmd.FetchNext())
            {
                SAString sVal = cmd[1].asString();
                printf("%s\n", sVal.GetMultiByteChars());
            }
        }


        SACommand cmd2(&con, _TSA("PRAGMA journal_mode = WAL"));
        cmd2.Execute();

        cmd.setCommandText(_TSA("SELECT name FROM sqlite_master"));
        cmd.Execute();

        while (cmd.isResultSet())
        {
            while (cmd.FetchNext())
            {
                SAString sVal = cmd[1].asString();
                printf("%s\n", sVal.GetMultiByteChars());
            }
        }
    }
    catch (SAException& x)
    {
        printf("ERROR:\n%s\n", x.ErrText().GetMultiByteChars());
    }

    return 0;
}

