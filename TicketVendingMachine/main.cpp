#include <Dxlib.h>
#include "Myself.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	if (lpMyself.GetSysSuccess())
	{
		lpMyself.Run();
	}
	return 0;
}