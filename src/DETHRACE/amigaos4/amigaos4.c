/*
** smarkusg: part of the code comes from mplayer 1.5 AOS
** License: GPL v2 or later
*/
#ifdef __AMIGAOS4__

#include <proto/dos.h>
#include <proto/icon.h>
#include <workbench/startup.h>
#include <stdio.h>
#include <string.h>
#include "amigaos4.h"
#include <proto/utility.h>
#include <string.h>
#include <stdlib.h>

char fullpath_dethrace[1024] = "";
char* SDL_FULL = NULL; //Tooltypes

void AmigOS_tooltype2(int argc, char *argv[])
{
 struct WBStartup *WBStartup = NULL;
 struct WBArg *WBArg=&WBStartup->sm_ArgList[0];
 struct DiskObject *icon = NULL;
 TEXT progname[512];

  // Ok is launched from cmd line, just do nothing
  if (argc)
  {
    return;
  }
 //  WBStartup
 WBStartup = (struct WBStartup *) argv;
  if (!WBStartup)
    {
     return ; // We never know !
    }
  else
   {
      // run in Workbench //javier code 
      NameFromLock( WBStartup->sm_ArgList->wa_Lock, fullpath_dethrace, sizeof(fullpath_dethrace) );
      Strlcpy( progname, WBStartup->sm_ArgList->wa_Name, sizeof(progname) );
      AddPart( fullpath_dethrace, FilePart(progname), sizeof(fullpath_dethrace) );
   }

  icon = GetDiskObject(fullpath_dethrace);
   if (icon)
    {
      STRPTR found;
      found = FindToolType(icon->do_ToolTypes, "HIRES");
      if (found)
       {
         free(SDL_FULL);
         SDL_FULL = strdup(found);
       }
      FreeDiskObject(icon);
      }
}


#endif //AOS4