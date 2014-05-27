#pragma once

namespace Hivemind
{
	template <typename T>
	class Writer
	{
	public:
		virtual ~Writer() {}

		virtual void write(const T& x) = 0;
	};
}
