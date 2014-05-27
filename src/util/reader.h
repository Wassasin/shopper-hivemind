#pragma once

namespace Hivemind
{
	template <typename T>
	class Reader
	{
	public:
		virtual ~Reader() {}

		virtual bool read(T& x_ref) = 0;
	};
}
