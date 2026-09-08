/**
 * Copyright (c) 2006-2023 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#ifndef LOVE_PROBE_H
#define LOVE_PROBE_H

#include "common/Module.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace love
{
namespace probe
{

struct Command
{
	std::string method;
	std::string target;
	std::string body;
};

class Probe : public Module
{
public:
	static constexpr std::size_t MAX_COMMANDS = 256;
	static constexpr std::size_t MAX_STATE_BYTES = 64 * 1024;

	Probe();
	~Probe() override;

	ModuleType getModuleType() const override
	{
		return M_PROBE;
	}
	const char *getName() const override
	{
		return "love.probe";
	}

	bool start(std::uint16_t port);
	void stop();
	bool running() const;
	std::uint16_t port() const;
	std::vector<Command> poll();
	bool setState(const std::string &state);

private:
	struct State;
	std::shared_ptr<State> state;
};

} // namespace probe
} // namespace love

#endif // LOVE_PROBE_H
