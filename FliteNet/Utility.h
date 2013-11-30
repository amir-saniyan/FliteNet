#pragma once

#include <string>

using namespace std;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace FliteNet
{
	ref class Utility abstract sealed
	{
	public:

		static string MarshalString(String^ str)
		{
			string stdStr;

			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(str)).ToPointer();
			stdStr = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));

			return stdStr;
		}
	};
}
