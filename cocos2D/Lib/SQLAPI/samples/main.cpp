#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include <SQLAPI.h>


int main(int argc, char *argv[])
{
    SAConnection con;
    try
    {
        con.Connect(_TSA("bedlam-m\\en2012@master"), _TSA(""), _TSA(""), SA_SQLServer_Client);
    }
    catch (SAException &x)
    {
        // show error message
        std::cout << "Err: " << (const char*)x.ErrText() << std::endl;
        return (1);
    }

    for (unsigned int i = 0; i < 0xFFFFFFFF; ++i)
    {
        try
        {
            SACommand cmd(&con, "select * from test_table");
            cmd.Execute();
            while (cmd.FetchNext())
            {
                printf("%s %s %s\n", cmd[1].asString().GetMultiByteChars(),
                    cmd[2].asString().GetMultiByteChars(),
                    cmd[3].asString().GetMultiByteChars());
            }
        }
        catch (SAException &x)
        {
            // show error message
            std::cout << "Err " << i << ": " << (const char*)x.ErrText() << std::endl;
        }
    }

    return (0);
}
