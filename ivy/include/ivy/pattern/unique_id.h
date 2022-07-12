#pragma once

#include "../utility/id_generator.h"

namespace ivy {
	namespace pattern {

		template<typename BaseT, typename IdGenT = utility::uint32_id_generator>
		class unique_id {
		public:
			typedef IdGenT id_generator_type;
			typedef typename id_generator_type::id_type id_type;

		public:
			virtual ~unique_id() noexcept;

			id_type id() const noexcept;

		protected:
			unique_id() noexcept;

			unique_id(unique_id&& _source) noexcept;
			unique_id(const unique_id& _other) noexcept = delete;

			unique_id& operator=(unique_id&& _source) noexcept;
			unique_id& operator=(const unique_id& _other) noexcept = delete;

		private:
			id_type m_id;

			static id_generator_type s_id_generator;
		};

	} // namespace pattern
} // namespace ivy

#include "../impl/pattern/unique_id.inl"
