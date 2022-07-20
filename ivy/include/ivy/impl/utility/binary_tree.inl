namespace ivy {
	namespace utility {

		template<typename T>
		i32 default_compare<T>::operator()(const T& _val1, const T& _val2) const noexcept {
			if (_val1 < _val2) {
				return -1;
			}
			if (_val1 > _val2) {
				return 1;
			}
			return 0;
		}

		template<typename T, typename CompareT>
		T& binary_tree<T, CompareT>::node::get_value() noexcept {
			return *(T*)value;
		}

		template<typename T, typename CompareT>
		const T& binary_tree<T, CompareT>::node::get_value() const noexcept {
			return *(const T*)value;
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::node::node_color binary_tree<T, CompareT>::node_color(const node* _node) noexcept {
			if (_node) {
				return _node->color;
			}
			return node::node_color::Black;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::node_swap_color(node* _node1, node* _node2) noexcept {
			typename node::node_color color1 = node_color(_node1);
			typename node::node_color color2 = node_color(_node2);

			if (_node1) {
				_node1->color = color2;
			}
			if (_node2) {
				_node2->color = color1;
			}

			return true;
		}
		
		template<typename T, typename CompareT>
		template<typename... Args>
		typename binary_tree<T, CompareT>::node* binary_tree<T, CompareT>::node_create(Args&&... _args) noexcept {
			node* res = (node*)platform::memory::allocate(sizeof(node));
			if (res) {
				new ((ptr)(res->value)) value_type(std::forward<Args>(_args)...);
				res->color  = node::node_color::Red;
				res->left   = nullptr;
				res->right  = nullptr;
				res->parent = nullptr;
			}

			return res;
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::node* binary_tree<T, CompareT>::node_copy(const node* _node, node* _parent) noexcept {
			if (!_node) {
				return nullptr;
			}

			node* new_node = (node*)platform::memory::allocate(sizeof(node));
			new ((ptr)new_node->value) value_type(_node->get_value());
			new_node->parent = _parent;
			new_node->color  = _node->color;
			new_node->left   = node_copy(_node->left, new_node);
			new_node->right  = node_copy(_node->right, new_node);

			return new_node;
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::node*& binary_tree<T, CompareT>::node_search(node*& _node, const value_type& _value) noexcept {
			node** current = &_node;

			while (*current) {
				i32 compare_result = m_compare(_value, (*current)->get_value());
				if (compare_result < 0) {
					current = &((*current)->left);
				}
				else if (compare_result > 0) {
					current = &((*current)->right);
				}
				else {
					break;
				}
			}

			return *current;
		}

		template<typename T, typename CompareT>
		const typename binary_tree<T, CompareT>::node*& binary_tree<T, CompareT>::node_search(const node*& _node, const value_type& _value) const noexcept {
			const node** current = &_node;

			while (*current) {
				i32 compare_result = m_compare(_value, (*current)->get_value());
				if (compare_result < 0) {
					current = &((*current)->left);
				}
				else if (compare_result > 0) {
					current = &((*current)->right);
				}
				else {
					break;
				}
			}

			return *current;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::node_lrotate(node*& _node) noexcept {
			node* right_child = _node->right;
			_node->right = right_child->left;

			if (_node->right != nullptr) {
				_node->right->parent = _node;
			}

			right_child->parent = _node->parent;

			if (!_node->parent) {
				m_root = right_child;
			}
			else if (_node == _node->parent->left) {
				_node->parent->left = right_child;
			}
			else {
				_node->parent->right = right_child;
			}

			right_child->left = _node;
			_node->parent = right_child;

			return true;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::node_rrotate(node*& _node) noexcept {
			node* left_child = _node->left;
			_node->left = left_child->right;

			if (_node->left != nullptr) {
				_node->left->parent = _node;
			}

			left_child->parent = _node->parent;

			if (!_node->parent) {
				m_root = left_child;
			}
			else if (_node == _node->parent->left) {
				_node->parent->left = left_child;
			}
			else {
				_node->parent->right = left_child;
			}

			left_child->right = _node;
			_node->parent = left_child;

			return true;
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::node* binary_tree<T, CompareT>::node_insert(node*& _root, node*& _node, node*& _inserted) noexcept {
			if (!_root) {
				_inserted = _node;
				return _node;
			}

			i32 compare_result = m_compare(_node->get_value(), _root->get_value());
			if (compare_result < 0) {
				_root->left = node_insert(_root->left, _node, _inserted);
				_root->left->parent = _root;
			}
			else if (compare_result > 0) {
				_root->right = node_insert(_root->right, _node, _inserted);
				_root->right->parent = _root;
			}
			else {
				_inserted = _root;
			}

			return _root;
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::node* binary_tree<T, CompareT>::node_remove(node*& _root, const value_type& _value) noexcept {
			node*& node_to_be_deleted = node_search(_root, _value);
			if (!node_to_be_deleted) {
				return nullptr;
			}
			node*  original_node = node_to_be_deleted;
			node*  x             = node_to_be_deleted;

			if (!node_to_be_deleted->left) {
				if (node_to_be_deleted->right) {
					node_to_be_deleted->right->parent = node_to_be_deleted->parent;
				}
				node_to_be_deleted = node_to_be_deleted->right;
			}
			else if (!node_to_be_deleted->right) {
				if (node_to_be_deleted->left) {
					node_to_be_deleted->left->parent = node_to_be_deleted->parent;
				}
				node_to_be_deleted = node_to_be_deleted->left;
			}
			else {
				node** y = &node_to_be_deleted->right;

				while ((*y)->left) {
					y = &(*y)->left;
				}
				x = *y;

				node* y_right  = (*y)->right;
				node* y_parent = (*y)->parent;
				
				(*y)->left = node_to_be_deleted->left;
				if ((*y)->left) {
					(*y)->left->parent = (*y);
				}

				if ((*y) != node_to_be_deleted->right) {
					(*y)->right = node_to_be_deleted->right;
					(*y)->right->parent = (*y);
				}

				(*y)->parent = node_to_be_deleted->parent;
				node_to_be_deleted = (*y);
				*y = y_right;
				if (y_right) {
					y_right->parent = y_parent;
				}
			}

			original_node->parent = nullptr;
			original_node->left   = nullptr;
			original_node->right  = nullptr;
			node_delete(original_node);
			return x;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::node_fix_insert(node*& _node) noexcept {
			node* parent      = nullptr;
			node* grandparent = nullptr;

			while (_node != m_root && node_color(_node) == node::node_color::Red && node_color(_node->parent) == node::node_color::Red) {
				parent      = _node->parent;
				grandparent = parent->parent;

				if (parent == grandparent->left) {
					node* uncle = grandparent->right;
					if (node_color(uncle) == node::node_color::Red) {
						uncle->color       = node::node_color::Black;
						parent->color      = node::node_color::Black;
						grandparent->color = node::node_color::Red;
						_node = grandparent;
					}
					else {
						if (_node == parent->right) {
							node_lrotate(parent);
							_node  = parent;
							parent = _node->parent;
						}
						node_rrotate(grandparent);
						node_swap_color(parent, grandparent);
						_node = parent;
					}
				}
				else {
					node* uncle = grandparent->left;
					if (node_color(uncle) == node::node_color::Red) {
						uncle->color       = node::node_color::Black;
						parent->color      = node::node_color::Black;
						grandparent->color = node::node_color::Red;
						_node = grandparent;
					}
					else {
						if (_node == parent->left) {
							node_rrotate(parent);
							_node  = parent;
							parent = _node->parent;
						}
						node_lrotate(grandparent);
						node_swap_color(parent, grandparent);
						_node = parent;
					}
				}
			}

			if (m_root) {
				m_root->color = node::node_color::Black;
			}
			return true;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::node_fix_remove(node* _node) noexcept {
			if (!_node) {
				return false;
			}

			node* s;

			while (_node != m_root && node_color(_node) == node::node_color::Black) {
				if (_node == _node->parent->left) {
					s = _node->parent->right;
					if (node_color(s) == node::node_color::Red) {
						s->color             = node::node_color::Black;
						_node->parent->color = node::node_color::Red;
						node_lrotate(_node->parent);
						s = _node->parent->right;
					}

					if (node_color(s->left) == node::node_color::Black && node_color(s->right) == node::node_color::Black) {
						s->color = node::node_color::Red;
						_node    = _node->parent;
					}
					else {
						if (node_color(s->right) == node::node_color::Black) {
							s->left->color = node::node_color::Black;
							s->color       = node::node_color::Red;
							node_rrotate(s);
							s = _node->parent->right;
						}

						s->color             = _node->parent->color;
						_node->parent->color = node::node_color::Black;
						s->right->color      = node::node_color::Black;
						node_lrotate(_node->parent);
						_node = m_root;
					}
				}
				else {
					s = _node->parent->left;
					if (node_color(s) == node::node_color::Red) {
						s->color             = node::node_color::Black;
						_node->parent->color = node::node_color::Red;
						node_rrotate(_node->parent);
						s = _node->parent->left;
					}

					if (node_color(s->left) == node::node_color::Black && node_color(s->right) == node::node_color::Black) {
						s->color = node::node_color::Red;
						_node    = _node->parent;
					}
					else {
						if (node_color(s->left) == node::node_color::Black) {
							s->right->color = node::node_color::Black;
							s->color        = node::node_color::Red;
							node_lrotate(s);
							s = _node->parent->left;
						}

						s->color = _node->parent->color;
						_node->parent->color = node::node_color::Black;
						s->left->color       = node::node_color::Black;
						node_rrotate(_node->parent);
						_node = m_root;
					}
				}
			}
			_node->color = node::node_color::Black;

			return true;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::node_delete(node*& _node) noexcept {
			if (_node) {
				node_delete(_node->left);
				node_delete(_node->right);
			
				_node->get_value().~value_type();
				platform::memory::deallocate(_node);
				_node = nullptr;
			}
			
			return true;
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::binary_tree() noexcept :
			m_root(nullptr) {
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::binary_tree(binary_tree&& _source) noexcept :
			m_root(_source.m_root) {
			_source.m_root = nullptr;
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::binary_tree(const binary_tree& _other) noexcept :
			m_root(node_copy(_other.m_root, nullptr)) {
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::~binary_tree() noexcept {
			node_delete(m_root);
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>& binary_tree<T, CompareT>::operator=(binary_tree&& _source) noexcept {
			if (this != &_source) {
				node_delete(m_root);
				m_root = _source.m_root;
				_source.m_root = nullptr;
			}

			return *this;
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>& binary_tree<T, CompareT>::operator=(const binary_tree& _other) noexcept {
			if (this != &_other) {
				node_delete(m_root);
				m_root = node_copy(_other.m_root, nullptr);
			}
			
			return *this;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::empty() const noexcept {
			return !m_root;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::clear() noexcept {
			node_delete(m_root);
			return true;
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::iterator binary_tree<T, CompareT>::begin() noexcept {
			if (!m_root) {
				return iterator(nullptr);
			}
			node* leftmost_node = m_root;
			while (leftmost_node->left) {
				leftmost_node = leftmost_node->left;
			}
			return iterator(leftmost_node);
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::const_iterator binary_tree<T, CompareT>::begin() const noexcept {
			if (!m_root) {
				return const_iterator(nullptr);
			}
			node* leftmost_node = m_root;
			while (leftmost_node->left) {
				leftmost_node = leftmost_node->left;
			}
			return const_iterator(leftmost_node);
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::iterator binary_tree<T, CompareT>::end() noexcept {
			return iterator(nullptr);
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::const_iterator binary_tree<T, CompareT>::end() const noexcept {
			return const_iterator(nullptr);
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::contains(const value_type& _value) const noexcept {
			const node* current = m_root;

			while (current) {
				i32 compare_result = m_compare(_value, current->get_value());
				if (compare_result < 0) {
					current = current->left;
				}
				else if (compare_result > 0) {
					current = current->right;
				}
				else {
					break;
				}
			}

			return current;
		}

		template<typename T, typename CompareT>
		template<typename... Args>
		T& binary_tree<T, CompareT>::emplace(Args&&... _args) noexcept {
			node* new_node = node_create(std::forward<Args>(_args)...);
			node* inserted = nullptr;
			m_root = node_insert(m_root, new_node, inserted);
			if (inserted == new_node) {
				node_fix_insert(new_node);
			}
			else {
				node_delete(new_node);
			}
			return inserted->get_value();
		}

		template<typename T, typename CompareT>
		T& binary_tree<T, CompareT>::insert(value_type&& _value) noexcept {
			return emplace(std::move(_value));
		}

		template<typename T, typename CompareT>
		T& binary_tree<T, CompareT>::insert(const value_type& _value) noexcept {
			return emplace(_value);
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::remove(const value_type& _value) noexcept {
			node* remove_node = node_remove(m_root, _value);
			if (!remove_node) {
				return false;
			}

			if (m_root) {
				node_fix_remove(remove_node);
			}

			return true;
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::iterator::iterator() noexcept :
			m_node(nullptr) {
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::iterator::iterator(node* _node) noexcept :
			m_node(_node) {
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::iterator::operator==(iterator _other) const noexcept {
			return m_node == _other.m_node;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::iterator::operator!=(iterator _other) const noexcept {
			return m_node != _other.m_node;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::iterator::operator==(const_iterator _other) const noexcept {
			return m_node == _other.m_node;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::iterator::operator!=(const_iterator _other) const noexcept {
			return m_node != _other.m_node;
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::iterator& binary_tree<T, CompareT>::iterator::operator++() noexcept {
			if (m_node) {
				if (m_node->right) {
					m_node = m_node->right;
					while (m_node->left) {
						m_node = m_node->left;
					}
				}
				else {
					while (m_node) {
						node* old_node = m_node;
						m_node = m_node->parent;
						if (m_node) {
							if (m_node->left == old_node) {
								break;
							}
						}
					}
				}
			}

			return *this;
		}


		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::iterator binary_tree<T, CompareT>::iterator::operator++(int) noexcept {
			iterator res = *this;
			++(*this);
			return res;
		}


		template<typename T, typename CompareT>
		T* binary_tree<T, CompareT>::iterator::operator->() const noexcept {
			return &(m_node->get_value());
		}


		template<typename T, typename CompareT>
		T& binary_tree<T, CompareT>::iterator::operator*() const noexcept {
			return m_node->get_value();
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::const_iterator::const_iterator() noexcept :
			m_node(nullptr) {
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::const_iterator::const_iterator(iterator _other) noexcept :
			m_node(_other.m_node) {
		}

		template<typename T, typename CompareT>
		binary_tree<T, CompareT>::const_iterator::const_iterator(const node* _node) noexcept :
			m_node(_node) {
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::const_iterator::operator==(const_iterator _other) const noexcept {
			return m_node == _other.m_node;
		}

		template<typename T, typename CompareT>
		b8 binary_tree<T, CompareT>::const_iterator::operator!=(const_iterator _other) const noexcept {
			return m_node != _other.m_node;
		}

		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::const_iterator& binary_tree<T, CompareT>::const_iterator::operator++() noexcept {
			if (m_node) {
				if (m_node->right) {
					m_node = m_node->right;
					while (m_node->left) {
						m_node = m_node->left;
					}
				}
				else {
					while (m_node) {
						const node* old_node = m_node;
						m_node = m_node->parent;
						if (m_node) {
							if (m_node->left == old_node) {
								break;
							}
						}
					}
				}
			}

			return *this;
		}


		template<typename T, typename CompareT>
		typename binary_tree<T, CompareT>::const_iterator binary_tree<T, CompareT>::const_iterator::operator++(int) noexcept {
			const_iterator res = *this;
			++(*this);
			return res;
		}


		template<typename T, typename CompareT>
		const T* binary_tree<T, CompareT>::const_iterator::operator->() const noexcept {
			return &(m_node->get_value());
		}


		template<typename T, typename CompareT>
		const T& binary_tree<T, CompareT>::const_iterator::operator*() const noexcept {
			return m_node->get_value();
		}


	} // namespace utility
} // namespace ivy
