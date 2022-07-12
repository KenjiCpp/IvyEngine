#pragma once

namespace ivy {
	namespace utility {

		template<typename UintT>
		class uint_id_generator {
		public:
			typedef UintT id_type;

		public:
			uint_id_generator() noexcept;

			uint_id_generator(uint_id_generator&& _source) noexcept;
			uint_id_generator(const uint_id_generator& _other) noexcept = delete;

			~uint_id_generator() noexcept;

			uint_id_generator& operator=(uint_id_generator&& _source) noexcept;
			uint_id_generator& operator=(const uint_id_generator& _other) noexcept = delete;

			id_type get() noexcept;

		private:
			id_type m_id;
		};

		typedef uint_id_generator<u8>  uint8_id_generator;
		typedef uint_id_generator<u16> uint16_id_generator;
		typedef uint_id_generator<u32> uint32_id_generator;
		typedef uint_id_generator<u64> uint64_id_generator;

		//class uuid_generator {

		//};

	} // namespace utility
} // namespace ivy

#include "impl/utility/id_generator.inl"
