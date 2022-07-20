#pragma once

#include "platform/memory.h"

#include <string>

namespace ivy {
	namespace utility {

		template<typename T>
		struct default_compare {
			i32 operator()(const T& _val1, const T& _val2) const noexcept;
		};

		template<typename T, typename CompareT = default_compare<T>>
		class binary_tree {
		public:
			typedef T        value_type;
			typedef CompareT compare_type;

			struct node {
				enum class node_color : u8 {
					Red, Black
				};

				u8         value[sizeof(value_type)];
				node_color color  = node_color::Red;
				node*      left   = nullptr;
				node*      right  = nullptr;
				node*	   parent = nullptr;

				value_type& get_value() noexcept;
				const value_type& get_value() const noexcept;
			};

			class const_iterator;

			class iterator {
			public:
				friend class binary_tree;
				friend class const_iterator;

			public:
				iterator() noexcept;

				b8 operator==(iterator _other) const noexcept;
				b8 operator!=(iterator _other) const noexcept;
				b8 operator==(const_iterator _other) const noexcept;
				b8 operator!=(const_iterator _other) const noexcept;

				iterator& operator++() noexcept;
				iterator operator++(int) noexcept;

				value_type* operator->() const noexcept;

				value_type& operator*() const noexcept;

			private:
				iterator(node* _node) noexcept;

			private:
				node* m_node;
			};

			class const_iterator {
			public:
				friend class binary_tree;
				friend class iterator;

			public:
				const_iterator() noexcept;

				const_iterator(iterator _other) noexcept;

				b8 operator==(const_iterator _other) const noexcept;
				b8 operator!=(const_iterator _other) const noexcept;

				const_iterator& operator++() noexcept;
				const_iterator operator++(int) noexcept;

				const value_type* operator->() const noexcept;

				const value_type& operator*() const noexcept;

			private:
				const_iterator(const node* _node) noexcept;

			private:
				const node* m_node;
			};

		private:
			typename node::node_color node_color(const node* _node) noexcept;
			b8 node_swap_color(node* _node1, node* _node2) noexcept;

			template<typename... Args>
			node* node_create(Args&&... _args) noexcept;

			node* node_copy(const node* _node, node* _parent) noexcept;

			node*& node_search(node*& _node, const value_type& _value) noexcept;
			const node*& node_search(const node*& _node, const value_type& _value) const noexcept;

			b8 node_lrotate(node*& _node) noexcept;
			b8 node_rrotate(node*& _node) noexcept;

			node* node_insert(node*& _root, node*& _node, node*& _inserted) noexcept;
			node* node_remove(node*& _root, const value_type& _value) noexcept;

			b8 node_fix_insert(node*& _node) noexcept;
			b8 node_fix_remove(node* _node) noexcept;

			b8 node_delete(node*& _node) noexcept;

		public:
			binary_tree() noexcept;

			binary_tree(binary_tree&& _source) noexcept;
			binary_tree(const binary_tree& _other) noexcept;

			~binary_tree() noexcept;

			binary_tree& operator=(binary_tree&& _source) noexcept;
			binary_tree& operator=(const binary_tree& _other) noexcept;

			b8 empty() const noexcept;

			b8 clear() noexcept;
			
			iterator begin() noexcept;
			const_iterator begin() const noexcept;

			iterator end() noexcept;
			const_iterator end() const noexcept;

			b8 contains(const value_type& _value) const noexcept;

			template<typename... Args>
			value_type& emplace(Args&&... _args) noexcept;

			value_type& insert(value_type&& _value) noexcept;
			value_type& insert(const value_type& _value) noexcept;

			b8 remove(const value_type& _value) noexcept;
			
		private:
			node*        m_root;
			compare_type m_compare;
		};

	} // namespace utility

	using utility::binary_tree;

} // namespace ivy

#include "../impl/utility/binary_tree.inl"
