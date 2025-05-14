#pragma once
#include <filesystem>
#include <iostream>
#include <string>

namespace Llp
{
	enum class CompilerFormat
	{
		Clang,
		Msvc,
		Gcc,
		Any
	};

	struct Location
	{
		size_t line = 0;
		size_t column = 0;
		size_t index = 0;

		Location& operator++()
		{
			index++;
			column++;
			return *this;
		}

		Location operator++(int)
		{
			Location old = *this;
			++*this;
			return old;
		}

		Location on_new_line()
		{
			Location old = *this;
			line++;
			column = 0;
			index++;
			return old;
		}
	};

	class ParserError
	{
	public:
		ParserError() : b_is_error(false)
		{
		}

		ParserError(const Location& in_location, std::string in_message) : location(in_location),
		                                                                   message(std::move(in_message)),
		                                                                   b_is_error(true)
		{
		}

		// Returns true if it is an error
		operator bool() const
		{
			return b_is_error;
		}

		void exit_on_error(const std::filesystem::path& path)
		{
			if (b_is_error)
			{
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
		[[nodiscard]] std::string compiler_format(const std::filesystem::path& path, std::string type = "error",
		                         CompilerFormat compiler_format = CompilerFormat::Gcc)
#else
		[[nodiscard]] std::string compiler_format(const std::filesystem::path& path, std::string type = "error",
		                                          CompilerFormat compiler_format = CompilerFormat::Any)
#endif
		{
			if (b_is_error)
			{
				switch (compiler_format)
				{
				case CompilerFormat::Msvc:
					return std::format("{}({},{}) {}: {}", path.generic_string(), location.line, location.column, type,
					                   message);
				case CompilerFormat::Gcc:
				case CompilerFormat::Clang:
				case CompilerFormat::Any:
					return std::format("{}:{}:{}: {}: {}", path.generic_string(), location.line, location.column, type,
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
