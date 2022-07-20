#pragma once

#include "binary_tree.h"
#include "nullable.h"

namespace ivy {
	namespace utility {

		template<typename KeyT, typename T>
		class map {
		public:
			typedef KeyT key_type;
			typedef T    value_type;

			class pair {
			public:
				friend class map;

			public:
				template<typename... Args>
				pair(const key_type& _key, Args&&... _args) noexcept;

				pair(pair&& _source) noexcept;
				pair(const pair& _other) noexcept;

				~pair() noexcept;

				pair& operator=(pair&& _source) noexcept;
				pair& operator=(const pair& _other) noexcept;

				b8 operator==(const pair& _other) const noexcept;
				b8 operator!=(const pair& _other) const noexcept;
				b8 operator>(const pair& _other) const noexcept;
				b8 operator<(const pair& _other) const noexcept;
				b8 operator>=(const pair& _other) const noexcept;
				b8 operator<=(const pair& _other) const noexcept;

				const key_type& key() const noexcept;

				value_type& value() noexcept;
				const value_type& value() const noexcept;

			private:
				pair(null_type _null, const key_type& _key) noexcept;

			private:
				const key_type       m_key;
				nullable<value_type> m_value;
			};

			typedef typename binary_tree<pair>::iterator       iterator;
			typedef typename binary_tree<pair>::const_iterator const_iterator;

		public:
			map() noexcept;

			map(map&& _source) noexcept;
			map(const map& _other) noexcept;

			~map() noexcept;

			map& operator=(map&& _source) noexcept;
			map& operator=(const map& _other) noexcept;

			b8 empty() const noexcept;

			iterator begin() noexcept;
			const_iterator begin() const noexcept;

			iterator end() noexcept;
			const_iterator end() const noexcept;

			value_type& operator[](const key_type& _key) noexcept;
			const value_type& operator[](const key_type& _key) const noexcept;

			b8 clear() noexcept;

			b8 contains(const key_type& _key) const noexcept;

			template<typename... Args>
			const value_type& emplace(const key_type& _key, Args&&... _args) noexcept;

			b8 remove(const key_type& _key) noexcept;

		private:
			mutable binary_tree<pair> m_tree;
		};

	} // namespace utility
	
	using utility::map;

} // namespace ivy

#include "../impl/utility/map.inl"
