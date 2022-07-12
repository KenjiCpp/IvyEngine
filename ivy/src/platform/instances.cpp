#include "platform/console.h"
#include "platform/common.h"
#include "platform/memory.h"
#include "platform/filesystem.h"

namespace ivy {
	namespace platform {

		namespace details {

			// Declare static instance of each module's implementation 
			// in the correct order

			console_impl console_impl::s_instance;
			common_impl  common_impl ::s_instance;
			memory_impl  memory_impl ::s_instance;

		} // namespace details

		class startup {
		public:
			startup() noexcept;
			~startup() noexcept;
		};

		startup::startup() noexcept {
			console::write("\n");
			// Execute something right after every module is initialized
		}

		startup::~startup() noexcept {
			console::write("\n");
			// Execute something right before every module is terminated
		}

		startup _startup;

	} // namespace platform
} // namespace ivy
