#ifndef DEBUG_HPP
#define DEBUG_HPP

#if CMAKE_BUILD_TYPE == Release

#include <QDebug>
#define DEBUG_LOG	qDebug()

#else

namespace Debug {

struct DebugLog {
	template<typename T>
	std::ostream& operator <<(const T& t)
	{
		return s;
	}

	std::ostream s;
};

}

#define DEBUG_LOG	Debug::DebugLog()

#endif /* CMAKE_BUILD_TYPE == Release */

#endif // DEBUG_HPP
