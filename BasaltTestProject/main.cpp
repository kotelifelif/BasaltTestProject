#include <cpr/cpr.h>

int main()
{
	auto r = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/version" });
	return 0;
}
