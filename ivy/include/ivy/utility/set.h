#pragma once

#include "binary_tree.h"

namespace ivy {
	namespace utility {

		template<typename T>
		class set {
		public:
			typedef T value_type;

			typedef typename binary_tree<const value_type>::iterator       iterator;
			typedef typename binary_tree<const value_type>::const_iterator const_iterator;

		public:
			set() noexcept;

			set(set&& _source) noexcept;
			set(const set& _other) noexcept;

			set(init_list<value_type> _init) noexcept;

			~set() noexcept;

			set& operator=(set&& _source) noexcept;
			set& operator=(const set& _other) noexcept;

			set& operator=(init_list<value_type> _init) noexcept;

			u32 size() const noexcept;
			b8 empty() const noexcept;

			iterator begin() noexcept;
			const_iterator begin() const noexcept;

			iterator end() noexcept;
			const_iterator end() const noexcept;

			b8 clear() noexcept;

			b8 contains(const value_type& _value) const noexcept;

			template<typename... Args>
			const value_type& emplace(Args&&... _args) noexcept;

			const value_type& insert(value_type&& _value) noexcept;
			const value_type& insert(const value_type& _value) noexcept;

			b8 remove(const value_type& _value) noexcept;

		private:
			binary_tree<const value_type> m_tree;
			u32                           m_size;
		};
		
		// TODO: Set operations: union, intersection, difference, symmetric_difference

		template<typename T>
		set<T> set_union(const set<T>& _set1, const set<T>& _set2) noexcept;

		template<typename T>
		set<T> set_intersection(const set<T>& _set1, const set<T>& _set2) noexcept;

		template<typename T>
		set<T> set_difference(const set<T>& _set1, const set<T>& _set2) noexcept;

		template<typename T>
		set<T> set_symmetric_difference(const set<T>& _set1, const set<T>& _set2) noexcept;

	} // namespace utility
} // namespace ivy

#include "../impl/utility/set.inl"
