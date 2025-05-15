#pragma once
#include <filesystem>
#include <format>
#include <iostream>
#include <string>

namespace Llp {
	enum class CompilerFormat {
		Clang,
		Msvc,
		Gcc,
		Any
	};

	class Location {
	public:
		[[nodiscard]] size_t get_index() const {
			return index;
		}

		[[nodiscard]] size_t get_line() const {
			return line;
		}

		[[nodiscard]] size_t get_column() const {
			return column;
		}

		Location &operator++() {
			index++;
			column++;
			return *this;
		}

		Location &operator+=(size_t offset) {
			index += offset;
			column += offset;
			return *this;
		}

		Location operator++(int) {
			Location old = *this;
			++*this;
			return old;
		}

		Location on_new_line() {
			Location old = *this;
			line++;
			column = 1;
			index++;
			return old;
		}

	private:
		size_t line = 1;
		size_t column = 1;
		size_t index = 0;
	};

	class ParserError {
	public:
		ParserError() : b_is_error(false) {
		}

		ParserError(const Location &in_location, std::string in_message) : location(in_location),
		                                                                   message(std::move(in_message)),
		                                                                   b_is_error(true) {
		}

		// Returns true if it is an error
		operator bool() const {
			return b_is_error;
		}

		void exit_on_error(const std::filesystem::path &path) {
			if (b_is_error) {
				std::cerr << compiler_format(path) << "\n";
				exit(EXIT_FAILURE);
			}
		}

#if __clang__
		[[nodiscard]] std::string compiler_format(const std::filesystem::path& path, std::string type = "error", 
			CompilerFormat compiler_format = CompilerFormat::Clang)
#elif _MSC_VER
		[[nodiscard]] std::string compiler_format(const std::filesystem::path& path, std::string type = "error",
		                                          CompilerFormat compiler_format = CompilerFormat::Msvc)
#elif __GNUC__
		[[nodiscard]] std::string compiler_format(const std::filesystem::path &path, std::string type = "error",
		                                          CompilerFormat compiler_format = CompilerFormat::Gcc)
#else
		[[nodiscard]] std::string compiler_format(const std::filesystem::path& path, std::string type = "error",
		                                          CompilerFormat compiler_format = CompilerFormat::Any)
#endif
		{
			if (b_is_error) {
				switch (compiler_format) {
					case CompilerFormat::Msvc:
						return std::format("{}({},{}) {}: {}", path.generic_string(), location.get_line(),
						                   location.get_column(), type,
						                   message);
					case CompilerFormat::Gcc:
					case CompilerFormat::Clang:
					case CompilerFormat::Any:
						return std::format("{}:{}:{}: {}: {}", path.generic_string(), location.get_line(),
						                   location.get_column(), type,
						                   message);
				}
			}
			return "No error !";
		}

		Location location;
		std::string message;

	private:
		bool b_is_error;
	};
}
