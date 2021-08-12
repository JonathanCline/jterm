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

	constexpr int xMax = 16;
	constexpr int yMax = 8;

	struct position
	{
		int x = 0;
		int y = 0;
	};
	struct user_pos
	{
		position at;
		position last;
	};

	user_pos pos{};
	auto& [x, y] = pos.at;

	jterm::set_callback(*tm, [](void* _up, char _text)
		{
			auto& _pos = *static_cast<user_pos*>(_up);
			auto& [x, y] = _pos.at;
			switch (_text)
			{
			case 'w':
				y = std::clamp(y - 1, 0, 16);
				break;
			case 'a':
				x = std::clamp(x - 1, 0, 16);
				break;
			case 's':
				y = std::clamp(y + 1, 0, 16);
				break;
			case 'd':
				x = std::clamp(x + 1, 0, 16);
				break;
			default:
				break;
			};
		}, &pos);

	while (true)
	{
		jterm::set_foreground_color(*tm, 255, 255, 255);
		jterm::put(*tm, ' ', pos.last.x, pos.last.y);
		pos.last = pos.at;

		jterm::set_foreground_color(*tm, 255, 0, 0);
		jterm::put(*tm, '@', x, y);

		jterm::set_foreground_color(*tm, 255, 255, 255);
		jterm::write(*tm, "            ", 2, 10);
		jterm::write(*tm, std::format("{}, {}", x, y), 2, 10);

		jterm::refresh(*tm);
		std::this_thread::sleep_for(16ms);
	};
	

	jterm::delete_terminal(tm);
	
	return 0;
};
