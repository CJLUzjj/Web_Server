//by Zhujunjie
#ifdef _MAIN_


#include "AppendFile.h"
#include <string>
int main()
{
	std::string name("log_test");
	AppendFile File(name);
	char line[17] = "this is a test\n";
	for (int i = 0; i < 100; i++) {
		File.append(line, sizeof line);
	}
	File.flush();
	return 0;
}

#endif // !_MAIN_