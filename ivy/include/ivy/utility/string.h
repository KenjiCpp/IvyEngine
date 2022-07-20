#pragma once

#include "../platform/memory.h"
#include "array.h"

namespace ivy {
	namespace utility {

		template<typename T> struct empty_string { };
		template<> struct empty_string<c8 > { static constexpr const c8 * value = ""; };
		template<> struct empty_string<c16> { static constexpr const c16* value = u""; };
		template<> struct empty_string<c32> { static constexpr const c32* value = U""; };

		template<typename T>
		class string {
		public:
			typedef T character_type;

			typedef character_type*       iterator;
			typedef const character_type* const_iterator;

			class view {
			public:
				typedef character_type character_type;

				typedef const character_type* iterator;
				typedef const character_type* const_iterator;

			public:
				view() noexcept;
				view(const view& _other) noexcept = default;

				view(const character_type* _cstr) noexcept;
				view(const character_type* _cstr, u32 _length) noexcept;
				view(const string& _string) noexcept;
				view(const string& _string, u32 _length) noexcept;

				view& operator=(const view& _other) noexcept = default;

				u32 length() const noexcept;
				b8 empty() const noexcept;

				const character_type* cstr() const noexcept;

				iterator begin() noexcept;
				const_iterator begin() const noexcept;

				iterator end() noexcept;
				const_iterator end() const noexcept;

				const character_type& front() noexcept;
				const character_type& front() const noexcept;

				const character_type& back() noexcept;
				const character_type& back() const noexcept;

				const character_type& operator[](u32 _idx) noexcept;
				const character_type& operator[](u32 _idx) const noexcept;

				string to_string() const noexcept;

				string substring(u32 _offset, u32 _length = npos) const noexcept;
				view subview(u32 _offset, u32 _length = npos) const noexcept;

			private:
				static u32 cstr_len(const character_type* _cstr) noexcept;

			private:
				const character_type* m_cstr;
				u32                   m_length;
			};

			static constexpr u32 npos = (u32)(-1);

		private:
			static constexpr u32 EXPAND_FACTOR = 2;

			static constexpr character_type TERMINATE = (character_type)(0);

		public:
			string() noexcept;
			string(string&& _source) noexcept;
			string(const string& _other) noexcept;

			explicit string(u32 _reserved_length) noexcept;
			
			string(character_type _char, u32 _length) noexcept;
			string(const character_type* _cstr) noexcept;
			string(const character_type* _cstr, u32 _length) noexcept;
			string(const string& _other, u32 _length) noexcept;
			string(const view& _view) noexcept;
			string(const view& _view, u32 _length) noexcept;

			~string() noexcept;

			string& operator=(string&& _source) noexcept;
			string& operator=(const string& _other) noexcept;

			string& operator=(const character_type* _cstr) noexcept;

			u32 length() const noexcept;
			u32 capacity() const noexcept;
			b8 empty() const noexcept;

			const character_type* cstr() const noexcept;

			iterator begin() noexcept;
			const_iterator begin() const noexcept;

			iterator end() noexcept;
			const_iterator end() const noexcept;

			character_type& front() noexcept;
			const character_type& front() const noexcept;

			character_type& back() noexcept;
			const character_type& back() const noexcept;

			character_type& operator[](u32 _idx) noexcept;
			const character_type& operator[](u32 _idx) const noexcept;

			b8 clear() noexcept;
			b8 reserve(u32 _reserved_length) noexcept;
			b8 shrink_to_fit() noexcept;

			b8 prepend(character_type _char) noexcept;
			b8 prepend(const character_type* _cstr) noexcept;
			b8 prepend(const string& _string) noexcept;
			b8 prepend(const view& _view) noexcept;

			b8 append(character_type _char) noexcept;
			b8 append(const character_type* _cstr) noexcept;
			b8 append(const string& _string) noexcept;
			b8 append(const view& _view) noexcept;

			string substring(u32 _offset, u32 _length = npos) const noexcept;	
			view subview(u32 _offset, u32 _length = npos) const noexcept;

			u32 find(character_type _char, u32 _offset) const noexcept;
			u32 find(const character_type* _cstr, u32 _offset) const noexcept;
			u32 find(const string& _string, u32 _offset) const noexcept;
			u32 find(const view& _view, u32 _offset) const noexcept;

			u32 find_first(character_type _char) const noexcept;
			u32 find_first(const character_type* _cstr) const noexcept;
			u32 find_first(const string& _string) const noexcept;
			u32 find_first(const view& _view) const noexcept;

			u32 find_last(character_type _char) const noexcept;
			u32 find_last(const character_type* _cstr) const noexcept;
			u32 find_last(const string& _string) const noexcept;
			u32 find_last(const view& _view) const noexcept;

			array<u32> find_all(character_type _char) const noexcept;
			array<u32> find_all(const character_type* _cstr) const noexcept;
			array<u32> find_all(const string& _string) const noexcept;
			array<u32> find_all(const view& _view) const noexcept;

			static i32 compare(const string& _string0, const string& _string1) noexcept;
			static i32 compare(const string& _string0, const view& _string1) noexcept;
			static i32 compare(const string& _string0, const character_type* _string1) noexcept;
			static i32 compare(const view& _string0, const string& _string1) noexcept;
			static i32 compare(const view& _string0, const view& _string1) noexcept;
			static i32 compare(const view& _string0, const character_type* _string1) noexcept;

		private:
			b8 expand_front() noexcept;
			b8 expand_back() noexcept;
			b8 expand_to_fit_front(u32 _desired_length) noexcept;
			b8 expand_to_fit_back(u32 _desired_length) noexcept;

			static u32 cstr_len(const character_type* _cstr) noexcept;
			static i32 cstr_cmp(const character_type* _cstr0, const character_type* _cstr1, u32 _length = npos) noexcept;

		private:
			character_type* m_data;
			u32			    m_offset;
			u32				m_length;
			u32				m_capacity;
		};

		template<typename T>
		b8 operator==(const string<T>& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator!=(const string<T>& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator>(const string<T>& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator<(const string<T>& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator>=(const string<T>& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator<=(const string<T>& _string0, const string<T>& _string1) noexcept;

		template<typename T>
		b8 operator==(const string<T>& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator!=(const string<T>& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator>(const string<T>& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator<(const string<T>& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator>=(const string<T>& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator<=(const string<T>& _string0, const typename string<T>::view& _string1) noexcept;

		template<typename T>
		b8 operator==(const string<T>& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator!=(const string<T>& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator>(const string<T>& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator<(const string<T>& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator>=(const string<T>& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator<=(const string<T>& _string0, const T* _string1) noexcept;

		template<typename T>
		b8 operator==(const typename string<T>::view& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator!=(const typename string<T>::view& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator>(const typename string<T>::view& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator<(const typename string<T>::view& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator>=(const typename string<T>::view& _string0, const string<T>& _string1) noexcept;
		template<typename T>
		b8 operator<=(const typename string<T>::view& _string0, const string<T>& _string1) noexcept;

		template<typename T>
		b8 operator==(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator!=(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator>(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator<(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator>=(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept;
		template<typename T>
		b8 operator<=(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept;

		template<typename T>
		b8 operator==(const typename string<T>::view& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator!=(const typename string<T>::view& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator>(const typename string<T>::view& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator<(const typename string<T>::view& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator>=(const typename string<T>::view& _string0, const T* _string1) noexcept;
		template<typename T>
		b8 operator<=(const typename string<T>::view& _string0, const T* _string1) noexcept;

		typedef string<c8>  string8;
		typedef string<c16> string16;
		typedef string<c32> string32;

	} // namespace strings

	using utility::string8;
	using utility::string16;
	using utility::string32;

} // namespace ivy

#include "../impl/utility/string.inl"
