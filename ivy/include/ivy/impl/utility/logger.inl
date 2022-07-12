namespace ivy {
	namespace utility {

		template<typename... Args>
		b8 logger::log(log_level _level, const template_type& _template, Args&&... _args) noexcept {
			auto fmt_func = [&_template, &_args...]() {
				return format(_template, std::forward<Args>(_args)...);
			};
			auto fmt = m_format_pool.enqueue(fmt_func);
			auto out_func = [this, _level, &fmt]() {
				return this->write(_level, fmt.get());
			};
			auto out = m_output_pool.enqueue(out_func);
			return out.get();
		}

	} // namespace utility
} // namespace ivy
