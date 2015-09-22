/*!
 * \file
 * Fichier main pour mingw pour palier l'absence de support de wmain par
 * mingw.
 */
#include <wchar.h>
#include "CocoSourcesCPP/Scanner.h"

extern int wmain(int argc, wchar_t *argv[]);

int main(int argc, char *argv[])
{
    wchar_t **wargv = new wchar_t*[argc];
    for(int i=0; i<argc; ++i)
    {
        wargv[i] = Coco::coco_string_create(argv[i]);
    }

    int result = wmain(argc, wargv);

    return result;
}
