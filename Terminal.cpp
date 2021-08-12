#include <jterm/jterm.h>

#include <jclib/ranges.h>

#include <format>
#include <thread>
#include <chrono>
#include <iostream>;

using namespace std::chrono_literals;



int main()
{
	auto tm = jterm::new_terminal();

	jterm::set(*tm, '@', 5, 5);
	jterm::set_color(*tm, 5, 5, jterm_Color{ 255, 0, 0, 0 }, jterm_Color{ 0, 0, 0, 0 });

	while (true)
	{
		std::this_thread::sleep_for(500ms);
	};

	jterm::delete_terminal(tm);
	
	return 0;
};
