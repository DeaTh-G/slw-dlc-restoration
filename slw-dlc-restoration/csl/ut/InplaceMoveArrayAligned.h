#pragma once

namespace csl
{
	namespace ut
	{
		template<typename T, size_t cap>
		struct InplaceMoveArrayAligned16
		{
			T* pBuffer;
			size_t length;
			size_t capacity;
			csl::fnd::IAllocator* pAllocator;
			alignas(16) char reserved[sizeof(T) * cap];
		};
	}
}