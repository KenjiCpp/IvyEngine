#include <cctype>

namespace ivy {
	namespace utility {

		template<typename T>
		string<T>::string() noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
		}

		template<typename T>
		string<T>::string(string&& _source) noexcept :
			m_data(_source.m_data),
			m_length(_source.m_length),
			m_offset(_source.m_offset),
			m_capacity(_source.m_capacity) {
			_source.m_data     = nullptr;
			_source.m_length   = 0;
			_source.m_offset   = 0;
			_source.m_capacity = 0;
		}

		template<typename T>
		string<T>::string(const string& _other) noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
			if (_other.m_data) {
				character_type* dst = (character_type*)platform::memory::allocate((_other.m_length + 1) * sizeof(character_type));
				assert(dst);

				m_data     = dst;
				m_length   = _other.m_length;
				m_capacity = _other.m_length;
				
				const character_type* src = _other.cstr();
				while (*src) {
					*(dst++) = *(src++);
				}
				*dst = TERMINATE;
			}
		}

		template<typename T>
		string<T>::string(u32 _reserved_length) noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
			if (_reserved_length) {
				character_type* new_data = (character_type*)platform::memory::allocate((_reserved_length * 2 + 1) * sizeof(character_type));
				assert(new_data);

				m_data     = new_data;
				m_length   = 0;
				m_offset   = _reserved_length;
				m_capacity = _reserved_length * 2;

				m_data[m_offset] = TERMINATE;
			}
		}

		template<typename T>
		string<T>::string(character_type _char, u32 _length) noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
			if (_length && !iscntrl(_char)) {
				character_type* dst = (character_type*)platform::memory::allocate((_length + 1) * sizeof(character_type));
				assert(dst);

				m_data     = dst;
				m_length   = _length;
				m_capacity = _length;

				while (_length--) {
					*(dst++) = _char;
				}

				*dst = TERMINATE;
			}
		}

		template<typename T>
		string<T>::string(const character_type* _cstr) noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
			u32 len = cstr_len(_cstr);
			if (len) {
				character_type* dst = (character_type*)platform::memory::allocate((len + 1) * sizeof(character_type));
				assert(dst);

				m_data     = dst;
				m_length   = len;
				m_capacity = len;

				const character_type* src = _cstr;
				while (*src) {
					*(dst++) = *(src++);
				}
				*dst = TERMINATE;
			}
		}

		template<typename T>
		string<T>::string(const character_type* _cstr, u32 _length) noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
			u32 len = std::min(cstr_len(_cstr), _length);
			if (len) {
				character_type* dst = (character_type*)platform::memory::allocate((len + 1) * sizeof(character_type));
				assert(dst);

				m_data     = dst;
				m_length   = len;
				m_capacity = len;

				const character_type* src = _cstr;
				while (*src && _length--) {
					*(dst++) = *(src++);
				}
				*dst = TERMINATE;
			}
		}

		template<typename T>
		string<T>::string(const string& _other, u32 _length) noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
			u32 len = std::min(_other.m_length, _length);
			if (len) {
				character_type* dst = (character_type*)platform::memory::allocate((len + 1) * sizeof(character_type));
				assert(dst);

				m_data     = dst;
				m_length   = len;
				m_capacity = len;

				const character_type* src = _other.cstr();
				while (*src && _length--) {
					*(dst++) = *(src++);
				}
				*dst = TERMINATE;
			}
		}

		template<typename T>
		string<T>::string(const view& _view) noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
			if (_view.length()) {
				character_type* dst = (character_type*)platform::memory::allocate((_view.length() + 1) * sizeof(character_type));
				assert(dst);

				m_data     = dst;
				m_length   = _view.length();
				m_capacity = _view.length();

				u32 len = _view.length();
				const character_type* src = _view.cstr();
				while (*src && len--) {
					*(dst++) = *(src++);
				}
				*dst = TERMINATE;
			}
		}

		template<typename T>
		string<T>::string(const view& _view, u32 _length) noexcept :
			m_data(nullptr),
			m_length(0),
			m_offset(0),
			m_capacity(0) {
			u32 len = std::min(_view.length(), _length);
			if (_view.length()) {
				character_type* dst = (character_type*)platform::memory::allocate((len + 1) * sizeof(character_type));
				assert(dst);

				m_data     = dst;
				m_length   = len;
				m_capacity = len;

				u32 _len = len;
				const character_type * src = view.cstr();
				while (*src && _len--) {
					*(dst++) = *(src++);
				}
				*dst = TERMINATE;
			}
		}

		template<typename T>
		string<T>::~string() noexcept {
			if (m_data) {
				platform::memory::deallocate(m_data);
			}
		}

		template<typename T>
		string<T>& string<T>::operator=(string&& _source) noexcept {
			if (this != &_source) {
				if (m_data) {
					platform::memory::deallocate(m_data);
				}

				m_data     = _source.m_data;
				m_length   = _source.m_length;
				m_offset   = _source.m_offset;
				m_capacity = _source.m_capacity;

				_source.m_data     = nullptr;
				_source.m_length   = 0;
				_source.m_offset   = 0;
				_source.m_capacity = 0;
			}

			return *this;
		}

		template<typename T>
		string<T>& string<T>::operator=(const string& _other) noexcept {
			if (this != &_other) {
				if (_other.m_length <= m_capacity) {
					m_offset = (m_capacity - _other.m_length) / 2;
					m_length = _other.m_length;

					character_type*       dst = m_data + m_offset;
					const character_type* src = _other.cstr();

					while (*src) {
						*(dst++) = *(src++);
					}
					*dst = TERMINATE;
				}
				else {
					if (m_data) {
						platform::memory::deallocate(m_data);
					}
					m_data     = nullptr;
					m_length   = 0;
					m_offset   = 0;
					m_capacity = 0;
					
					if (_other.m_length) {
						character_type* dst = (character_type*)platform::memory::allocate((_other.m_length + 1) * sizeof(character_type));
						assert(dst);

						m_data     = dst;
						m_length   = _other.m_length;
						m_capacity = _other.m_length;

						const character_type* src = _other.cstr();
						while (*src) {
							*(dst++) = *(src++);
						}
						*dst = TERMINATE;
					}
				}
			}

			return *this;
		}

		template<typename T>
		string<T>& string<T>::operator=(const character_type* _cstr) noexcept {
			u32 len = cstr_len(_cstr);
			if (len <= m_capacity) {
				m_offset = (m_capacity - len) / 2;
				m_length = len;

				character_type*       dst = m_data + m_offset;
				const character_type* src = _cstr;

				while (*src) {
					*(dst++) = *(src++);
				}
				*dst = TERMINATE;
			}
			else {
				if (m_data) {
					platform::memory::deallocate(m_data);
				}
				m_data     = nullptr;
				m_length   = 0;
				m_offset   = 0;
				m_capacity = 0;

				if (len) {
					character_type* dst = (character_type*)platform::memory::allocate((len + 1) * sizeof(character_type));
					assert(dst);

					m_data     = dst;
					m_length   = len;
					m_capacity = len;

					const character_type* src = _cstr;
					while (*src) {
						*(dst++) = *(src++);
					}
					*dst = TERMINATE;
				}
			}

			return *this;
		}

		template<typename T>
		u32 string<T>::length() const noexcept {
			return m_length;
		}

		template<typename T>
		u32 string<T>::capacity() const noexcept {
			return m_capacity;
		}

		template<typename T>
		b8 string<T>::empty() const noexcept {
			return (m_length == 0);
		}

		template<typename T>
		const T* string<T>::cstr() const noexcept {
			if (!m_data) {
				return empty_string<T>::value;
			}
			return m_data + m_offset;
		}

		template<typename T>
		T* string<T>::begin() noexcept {
			return m_data + m_offset;
		}

		template<typename T>
		const T* string<T>::begin() const noexcept {
			return m_data + m_offset;
		}

		template<typename T>
		T* string<T>::end() noexcept {
			return m_data + m_offset + m_length;
		}

		template<typename T>
		const T* string<T>::end() const noexcept {
			return m_data + m_offset + m_length;
		}

		template<typename T>
		T& string<T>::front() noexcept {
			assert(m_length > 0);
			return m_data[m_offset];
		}

		template<typename T>
		const T& string<T>::front() const noexcept {
			assert(m_length > 0);
			return m_data[m_offset];
		}

		template<typename T>
		T& string<T>::back() noexcept {
			assert(m_length > 0);
			return m_data[m_offset + m_length - 1];
		}

		template<typename T>
		const T& string<T>::back() const noexcept {
			assert(m_length > 0);
			return m_data[m_offset + m_length - 1];
		}

		template<typename T>
		T& string<T>::operator[](u32 _idx) noexcept {
			assert(m_length > _idx);
			return m_data[m_offset + _idx];
		}

		template<typename T>
		const T& string<T>::operator[](u32 _idx) const noexcept {
			assert(m_length > _idx);
			return m_data[m_offset + _idx];
		}

		template<typename T>
		b8 string<T>::clear() noexcept {
			if (m_data) {
				m_data[m_offset] = TERMINATE;
				m_length = 0;
			}
			return true;
		}

		template<typename T>
		b8 string<T>::reserve(u32 _reserved_length) noexcept {
			if (_reserved_length * 2 > m_capacity) {
				character_type* dst = (character_type*)platform::memory::allocate((_reserved_length * 2 + 1) * sizeof(character_type));
				assert(dst);

				u32 old_offset = m_offset;

				m_capacity = _reserved_length * 2;
				m_offset   = (m_capacity - m_length) / 2;

				character_type* new_data = dst;
				if (m_data) {
					character_type* src = m_data + old_offset;

					dst += m_offset;
					while (*src) {
						*(dst++) = *(src++);
					}
					*dst = TERMINATE;

					platform::memory::deallocate(m_data);
				}
				m_data = new_data;

				return true;
			}

			return false;
		}

		template<typename T>
		b8 string<T>::shrink_to_fit() noexcept {
			if (m_length < m_capacity) {
				if (m_length) {
					character_type* new_data = (character_type*)platform::memory::allocate((m_length + 1) * sizeof(character_type));
					assert(new_data);

					character_type* dst = new_data;
					character_type* src = m_data + m_offset;

					while (*src) {
						*(dst++) = *(src++);
					}
					*dst = TERMINATE;

					platform::memory::deallocate(m_data);
					m_data = new_data;
					m_offset = 0;
					m_capacity = m_length;
				}
				else {
					platform::memory::deallocate(m_data);
					m_data     = nullptr;
					m_length   = 0;
					m_offset   = 0;
					m_capacity = 0;
				}

				return true;
			}

			return false;
		}

		template<typename T>
		b8 string<T>::prepend(character_type _char) noexcept {
			if (m_offset == 0) {
				expand_front();
			}

			m_data[--m_offset] = _char;
			++m_length;

			return true;
		}

		template<typename T>
		b8 string<T>::prepend(const character_type* _cstr) noexcept {
			u32 len = cstr_len(_cstr);

			if (m_offset < len) {
				expand_to_fit_front(m_length + len);
			}

			const character_type* src = _cstr;
			character_type* dst = m_data + m_offset - len;

			while (*src) {
				*(dst++) = *(src++);
			}
			m_length += len;
			m_offset -= len;

			return true;
		}

		template<typename T>
		b8 string<T>::prepend(const string& _string) noexcept {
			if (m_offset < _string.m_length) {
				expand_to_fit_front(m_length + _string.m_length);
			}

			const character_type* src = _string.cstr();
			character_type* dst = m_data + m_offset - _string.m_length;

			while (*src) {
				*(dst++) = *(src++);
			}
			m_length += _string.m_length;
			m_offset -= _string.m_length;

			return true;
		}

		template<typename T>
		b8 string<T>::prepend(const view& _view) noexcept {
			if (m_offset < _view.length()) {
				expand_to_fit_front(m_length + _view.length());
			}

			const character_type* src = _view.cstr();
			character_type*       dst = m_data + m_offset - _view.length();
			u32 len = _view.length();

			while (*src && len--) {
				*(dst++) = *(src++);
			}
			m_length += _view.length();
			m_offset -= _view.length();

			return true;
		}

		template<typename T>
		b8 string<T>::append(character_type _char) noexcept {
			if (m_offset + m_length >= m_capacity) {
				expand_back();
;			}

			m_data[m_offset + m_length++] = _char;
			m_data[m_offset + m_length]   = TERMINATE;

			return true;
		}

		template<typename T>
		b8 string<T>::append(const character_type* _cstr) noexcept {
			u32 len = cstr_len(_cstr);
			if (len) {
				if (m_offset + m_length + len >= m_capacity) {
					expand_to_fit_back(m_length + len);
				}

				const character_type* src = _cstr;
				character_type* dst = m_data + m_offset + m_length;

				while (*src) {
					*(dst++) = *(src++);
				}
				m_length += len;
				m_data[m_offset + m_length] = TERMINATE;
			}

			return true;
		}

		template<typename T>
		b8 string<T>::append(const string& _string) noexcept {
			if (_string.m_length) {
				if (m_offset + m_length + _string.m_length >= m_capacity) {
					expand_to_fit_back(m_length + _string.m_length);
				}

				const character_type* src = _string.cstr();
				character_type* dst = m_data + m_offset + m_length;

				while (*src) {
					*(dst++) = *(src++);
				}
				m_length += _string.m_length;
				m_data[m_offset + m_length] = TERMINATE;
			}

			return true;
		}

		template<typename T>
		b8 string<T>::append(const view& _view) noexcept {
			if (_view.length()) {
				if (m_offset + m_length + _view.length() >= m_capacity) {
					expand_to_fit_back(m_length + _view.length());
				}

				const character_type* src = _view.cstr();
				character_type* dst = m_data + m_offset + m_length;
				u32 len = _view.length();

				while (*src && len--) {
					*(dst++) = *(src++);
				}
				m_length += _view.length();
				m_data[m_offset + m_length] = TERMINATE;
			}

			return true;
		}

		template<typename T>
		string<T> string<T>::substring(u32 _offset, u32 _length) const noexcept {
			u32 len = std::min(_length, m_length - _offset);
			string res(len);

			const character_type* src = m_data + m_offset + _offset;
			while (len--) {
				res.append(*(src++));
			}

			res.shrink_to_fit();
			return res;
		}

		template<typename T>
		typename string<T>::view string<T>::subview(u32 _offset, u32 _length) const noexcept {
			return view(m_data + m_offset + _offset, _length);
		}

		template<typename T>
		u32 string<T>::find(character_type _char, u32 _offset) const noexcept {
			const character_type* s = m_data + m_offset + _offset;
			while (*s) {
				if (*s == _char) {
					return s - (m_data + m_offset);
				}
				++s;
			}
			
			return npos;
		}

		template<typename T>
		u32 string<T>::find(const character_type* _cstr, u32 _offset) const noexcept {
			const character_type* s = m_data + m_offset + _offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _cstr;

				while (*s2) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				++s;
			}

			return npos;
		}

		template<typename T>
		u32 string<T>::find(const string& _string, u32 _offset) const noexcept {
			const character_type* s = m_data + m_offset + _offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _string.cstr();

				while (*s2) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				++s;
			}

			return npos;
		}
		template<typename T>
		u32 string<T>::find(const view& _view, u32 _offset) const noexcept {
			const character_type* s = m_data + m_offset + _offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _view.cstr();
				u32 len = _view.length();

				while (*s2 && len--) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				++s;
			}

			return npos;
		}


		template<typename T>
		u32 string<T>::find_first(character_type _char) const noexcept {
			const character_type* s = m_data + m_offset;
			while (*s) {
				if (*s == _char) {
					return s - (m_data + m_offset);
				}
				++s;
			}

			return npos;
		}

		template<typename T>
		u32 string<T>::find_first(const character_type* _cstr) const noexcept {
			const character_type* s = m_data + m_offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _cstr;

				while (*s2) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				++s;
			}

			return npos;
		}

		template<typename T>
		u32 string<T>::find_first(const string& _string) const noexcept {
			const character_type* s = m_data + m_offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _string.cstr();

				while (*s2) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				++s;
			}

			return npos;
		}

		template<typename T>
		u32 string<T>::find_first(const view& _view) const noexcept {
			const character_type* s = m_data + m_offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _view.cstr();
				u32 len = _view.length();

				while (*s2 && len--) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				++s;
			}

			return npos;
		}

		template<typename T>
		u32 string<T>::find_last(character_type _char) const noexcept {
			const character_type* s = m_data + m_offset + m_length - 1;
			while (s != m_data + m_offset - 1) {
				if (*s == _char) {
					return s - (m_data + m_offset);
				}
				--s;
			}

			return npos;
		}

		template<typename T>
		u32 string<T>::find_last(const character_type* _cstr) const noexcept {
			u32 len = cstr_len(_cstr);
			const character_type* s = m_data + m_offset + m_length - len;
			while (s != m_data + m_offset - 1) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _cstr;
				
				while (*s2) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				--s;
			}

			return npos;
		}

		template<typename T>
		u32 string<T>::find_last(const string& _string) const noexcept {
			const character_type* s = m_data + m_offset + m_length - _string.m_length;
			while (s != m_data + m_offset - 1) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _string.cstr();

				while (*s2) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				--s;
			}

			return npos;
		}

		template<typename T>
		u32 string<T>::find_last(const view& _view) const noexcept {
			const character_type* s = m_data + m_offset + m_length - _view.length();
			while (s != m_data + m_offset - 1) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _view.cstr();
				u32 len = _view.length();

				while (*s2 && len--) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					return s - (m_data + m_offset);
				}
				--s;
			}

			return npos;
		}

		template<typename T>
		array<u32> string<T>::find_all(character_type _char) const noexcept {
			array<u32> res(m_length);

			const character_type* s = m_data + m_offset;
			while (*s) {
				if (*s == _char) {
					res.emplace_back(s - (m_data + m_offset));
				}
				++s;
			}

			res.shrink_to_fit();
			return res;
		}

		template<typename T>
		array<u32> string<T>::find_all(const character_type* _cstr) const noexcept {
			array<u32> res(m_length);

			const character_type* s = m_data + m_offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _cstr;

				while (*s2) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					res.emplace_back(s - (m_data + m_offset));
				}
				++s;
			}

			res.shrink_to_fit();
			return res;
		}

		template<typename T>
		array<u32> string<T>::find_all(const string& _string) const noexcept {
			array<u32> res(m_length);

			const character_type* s = m_data + m_offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _string.cstr();

				while (*s2) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					res.emplace_back(s - (m_data + m_offset));
				}
				++s;
			}

			res.shrink_to_fit();
			return res;
		}

		template<typename T>
		array<u32> string<T>::find_all(const view& _view) const noexcept {
			array<u32> res(m_length);

			const character_type* s = m_data + m_offset;
			while (*s) {
				b8 found = true;

				const character_type* s1 = s;
				const character_type* s2 = _view.cstr();
				u32 len = _view.length();

				while (*s2 && len--) {
					if (*(s1++) != *(s2++)) {
						found = false;
						break;
					}
				}

				if (found) {
					res.emplace_back(s - (m_data + m_offset));
				}
				++s;
			}

			res.shrink_to_fit();
			return res;
		}

		template<typename T>
		i32 string<T>::compare(const string& _string0, const string& _string1) noexcept {
			return cstr_cmp(_string0.cstr(), _string1.cstr());
		}

		template<typename T>
		i32 string<T>::compare(const string& _string0, const view& _string1) noexcept {
			return cstr_cmp(_string0.cstr(), _string1.cstr(), _string1.length());
		}

		template<typename T>
		i32 string<T>::compare(const string& _string0, const character_type* _string1) noexcept {
			return cstr_cmp(_string0.cstr(), _string1);
		}

		template<typename T>
		i32 string<T>::compare(const view& _string0, const string& _string1) noexcept {
			return cstr_cmp(_string0.cstr(), _string1.cstr(), _string0.length());
		}

		template<typename T>
		i32 string<T>::compare(const view& _string0, const view& _string1) noexcept {
			return cstr_cmp(_string0.cstr(), _string1.cstr(), std::min(_string0.length(), _string1.length()));
		}

		template<typename T>
		i32 string<T>::compare(const view& _string0, const character_type* _string1) noexcept {
			return cstr_cmp(_string0.cstr(), _string1, _string0.length());
		}

		template<typename T>
		b8 string<T>::expand_front() noexcept {
			u32 new_capacity = m_capacity * EXPAND_FACTOR + 1;
			character_type* new_data = (character_type*)platform::memory::allocate((new_capacity + 1) * sizeof(character_type));
			assert(new_data);

			u32 new_offset = m_offset + (new_capacity - m_capacity);

			if (m_data) {
				character_type* src = m_data + m_offset;
				character_type* dst = new_data + new_offset;

				while (*src) {
					*(dst++) = *(src++);
				}

				platform::memory::deallocate(m_data);
			}
			new_data[new_offset + m_length] = TERMINATE;

			m_data     = new_data;
			m_offset   = new_offset;
			m_capacity = new_capacity;

			return true;
		}

		template<typename T>
		b8 string<T>::expand_back() noexcept {
			u32 new_capacity = m_capacity * EXPAND_FACTOR + 1;
			character_type* new_data = (character_type*)platform::memory::allocate((new_capacity + 1) * sizeof(character_type));
			assert(new_data);

			if (m_data) {
				character_type* src = m_data + m_offset;
				character_type* dst = new_data + m_offset;

				while (*src) {
					*(dst++) = *(src++);
				}

				platform::memory::deallocate(m_data);
			}
			new_data[m_offset + m_length] = TERMINATE;

			m_data     = new_data;
			m_capacity = new_capacity;

			return true;
		}

		template<typename T>
		b8 string<T>::expand_to_fit_front(u32 _desired_length) noexcept {
			u32 new_capacity = m_capacity;
			while (new_capacity < m_offset + _desired_length) {
				new_capacity = new_capacity * EXPAND_FACTOR + 1;
			}
			if (new_capacity == m_capacity) {
				return false;
			}

			character_type* new_data = (character_type*)platform::memory::allocate((new_capacity + 1) * sizeof(character_type));
			assert(new_data);

			u32 new_offset = new_capacity - m_capacity;

			if (m_data) {
				character_type* src = m_data + m_offset;
				character_type* dst = new_data + new_offset;

				while (*src) {
					*(dst++) = *(src++);
				}

				platform::memory::deallocate(m_data);
			}
			new_data[new_offset + m_length] = TERMINATE;

			m_data     = new_data;
			m_offset   = new_offset;
			m_capacity = new_capacity;

			return true;
		}

		template<typename T>
		b8 string<T>::expand_to_fit_back(u32 _desired_length) noexcept {
			u32 new_capacity = m_capacity;
			u32 new_offset   = m_offset;
			while (new_capacity < new_offset + _desired_length) {
				new_capacity = new_capacity * EXPAND_FACTOR + 1;
			}
			if (new_capacity == m_capacity) {
				return false;
			}

			character_type* new_data = (character_type*)platform::memory::allocate((new_capacity + 1) * sizeof(character_type));
			assert(new_data);
			
			if (m_data) {
				character_type* src = m_data + m_offset;
				character_type* dst = new_data + new_offset;

				while (*src) {
					*(dst++) = *(src++);
				}

				platform::memory::deallocate(m_data);
			}
			new_data[new_offset + m_length] = TERMINATE;

			m_data     = new_data;
			m_offset   = new_offset;
			m_capacity = new_capacity;

			return true;
		}

		template<typename T>
		u32 string<T>::cstr_len(const character_type* _cstr) noexcept {
			const character_type* s = _cstr;
			for (; *s; ++s);
			return s - _cstr;
		}

		template<typename T>
		i32 string<T>::cstr_cmp(const character_type* _cstr0, const character_type* _cstr1, u32 _length) noexcept {
			const u8* p1 = (const u8*)_cstr0;
			const u8* p2 = (const u8*)_cstr1;
			while (_length-- && *p1 && *p1 == *p2) ++p1, ++p2;
			return (*p1 > *p2) - (*p2 > *p1);
		}

		template<typename T>
		string<T>::view::view() noexcept :
			m_cstr(empty_string<T>::value),
			m_length(0) {
		}

		template<typename T>
		string<T>::view::view(const character_type* _cstr) noexcept :
			m_cstr(_cstr),
			m_length(cstr_len(_cstr)) {
		}

		template<typename T>
		string<T>::view::view(const character_type* _cstr, u32 _length) noexcept :
			m_cstr(_cstr),
			m_length(std::min(cstr_len(_cstr), _length)) {
		}

		template<typename T>
		string<T>::view::view(const string& _string) noexcept :
			m_cstr(_string.cstr()),
			m_length(_string.length()) {
		}

		template<typename T>
		string<T>::view::view(const string& _string, u32 _length) noexcept :
			m_cstr(_string.cstr()),
			m_length(std::min(_string.length(), _length)) {
		}

		template<typename T>
		u32 string<T>::view::length() const noexcept {
			return m_length;
		}

		template<typename T>
		b8 string<T>::view::empty() const noexcept {
			return (m_length == 0);
		}

		template<typename T>
		const T* string<T>::view::cstr() const noexcept {
			return m_cstr;
		}

		template<typename T>
		const T* string<T>::view::begin() noexcept {
			return m_cstr;
		}

		template<typename T>
		const T* string<T>::view::begin() const noexcept {
			return m_cstr;
		}

		template<typename T>
		const T* string<T>::view::end() noexcept {
			return m_cstr + m_length;
		}

		template<typename T>
		const T* string<T>::view::end() const noexcept {
			return m_cstr + m_length;
		}

		template<typename T>
		const T& string<T>::view::front() noexcept {
			assert(m_length > 0);
			return m_cstr[0];
		}

		template<typename T>
		const T& string<T>::view::front() const noexcept {
			assert(m_length > 0);
			return m_cstr[0];
		}

		template<typename T>
		const T& string<T>::view::back() noexcept {
			assert(m_length > 0);
			return m_cstr[m_length - 1];
		}

		template<typename T>
		const T& string<T>::view::back() const noexcept {
			assert(m_length > 0);
			return m_cstr[m_length - 1];
		}

		template<typename T>
		const T& string<T>::view::operator[](u32 _idx) noexcept {
			assert(m_length > _idx);
			return m_cstr[_idx];
		}

		template<typename T>
		const T& string<T>::view::operator[](u32 _idx) const noexcept {
			assert(m_length > _idx);
			return m_cstr[_idx];
		}

		template<typename T>
		string<T> string<T>::view::to_string() const noexcept {
			return string(m_cstr, m_length);
		}

		template<typename T>
		string<T> string<T>::view::substring(u32 _offset, u32 _length) const noexcept {
			return string(m_cstr + _offset, std::min(m_length - _offset, _length));
		}
		template<typename T>
		typename string<T>::view string<T>::view::subview(u32 _offset, u32 _length) const noexcept {
			return view(m_cstr + _offset, std::min(m_length - _offset, _length));
		}

		template<typename T>
		u32 string<T>::view::cstr_len(const character_type* _cstr) noexcept {
			const character_type* s = _cstr;
			for (; *s; ++s);
			return s - _cstr;
		}

		template<typename T>
		b8 operator==(const string<T>& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) == 0);
		}

		template<typename T>
		b8 operator!=(const string<T>& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) != 0);
		}

		template<typename T>
		b8 operator>(const string<T>& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) > 0);
		}

		template<typename T>
		b8 operator<(const string<T>& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) < 0);
		}

		template<typename T>
		b8 operator>=(const string<T>& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) >= 0);
		}

		template<typename T>
		b8 operator<=(const string<T>& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) <= 0);
		}

		template<typename T>
		b8 operator==(const string<T>& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) == 0);
		}

		template<typename T>
		b8 operator!=(const string<T>& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) != 0);
		}

		template<typename T>
		b8 operator>(const string<T>& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) > 0);
		}

		template<typename T>
		b8 operator<(const string<T>& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) < 0);
		}

		template<typename T>
		b8 operator>=(const string<T>& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) >= 0);
		}

		template<typename T>
		b8 operator<=(const string<T>& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) <= 0);
		}

		template<typename T>
		b8 operator==(const string<T>& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) == 0);
		}

		template<typename T>
		b8 operator!=(const string<T>& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) != 0);
		}

		template<typename T>
		b8 operator>(const string<T>& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) > 0);
		}

		template<typename T>
		b8 operator<(const string<T>& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) < 0);
		}

		template<typename T>
		b8 operator>=(const string<T>& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) >= 0);
		}

		template<typename T>
		b8 operator<=(const string<T>& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) <= 0);
		}




		template<typename T>
		b8 operator==(const typename string<T>::view& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) == 0);
		}

		template<typename T>
		b8 operator!=(const typename string<T>::view& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) != 0);
		}

		template<typename T>
		b8 operator>(const typename string<T>::view& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) > 0);
		}

		template<typename T>
		b8 operator<(const typename string<T>::view& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) < 0);
		}

		template<typename T>
		b8 operator>=(const typename string<T>::view& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) >= 0);
		}

		template<typename T>
		b8 operator<=(const typename string<T>::view& _string0, const string<T>& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) <= 0);
		}

		template<typename T>
		b8 operator==(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) == 0);
		}

		template<typename T>
		b8 operator!=(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) != 0);
		}

		template<typename T>
		b8 operator>(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) > 0);
		}

		template<typename T>
		b8 operator<(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) < 0);
		}

		template<typename T>
		b8 operator>=(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) >= 0);
		}

		template<typename T>
		b8 operator<=(const typename string<T>::view& _string0, const typename string<T>::view& _string1) noexcept {
			return (string<T>::compare(_string0, _string1) <= 0);
		}

		template<typename T>
		b8 operator==(const typename string<T>::view& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) == 0);
		}

		template<typename T>
		b8 operator!=(const typename string<T>::view& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) != 0);
		}

		template<typename T>
		b8 operator>(const typename string<T>::view& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) > 0);
		}

		template<typename T>
		b8 operator<(const typename string<T>::view& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) < 0);
		}

		template<typename T>
		b8 operator>=(const typename string<T>::view& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) >= 0);
		}

		template<typename T>
		b8 operator<=(const typename string<T>::view& _string0, const T* _string1) noexcept {
			return (string<T>::compare(_string0, _string1) <= 0);
		}

	} // namespace strings
} // namespace ivy
