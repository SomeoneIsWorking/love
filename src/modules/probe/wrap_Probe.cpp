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

#include "wrap_Probe.h"

#include <cstdint>

namespace love
{
namespace probe
{

#define instance() (Module::getInstance<Probe>(Module::M_PROBE))

static int w_start(lua_State *L)
{
	const lua_Integer requested_port = luaL_optinteger(L, 1, 0);
	if (requested_port < 0 || requested_port > 0xFFFF)
		return luaL_error(L, "probe port must be between 0 and 65535");
	bool started = false;
	luax_catchexcept(L, [&]() { started = instance()->start((std::uint16_t)requested_port); });
	lua_pushboolean(L, started);
	return 1;
}

static int w_stop(lua_State *L)
{
	instance()->stop();
	return 0;
}

static int w_running(lua_State *L)
{
	lua_pushboolean(L, instance()->running());
	return 1;
}

static int w_port(lua_State *L)
{
	lua_pushinteger(L, instance()->port());
	return 1;
}

static int w_poll(lua_State *L)
{
	std::vector<Command> commands = instance()->poll();
	lua_createtable(L, (int)commands.size(), 0);
	int index = 1;
	for (const Command &command : commands)
	{
		lua_createtable(L, 0, 3);
		lua_pushlstring(L, command.method.data(), command.method.size());
		lua_setfield(L, -2, "method");
		lua_pushlstring(L, command.target.data(), command.target.size());
		lua_setfield(L, -2, "target");
		lua_pushlstring(L, command.body.data(), command.body.size());
		lua_setfield(L, -2, "body");
		lua_rawseti(L, -2, index++);
	}
	return 1;
}

static int w_setState(lua_State *L)
{
	size_t length = 0;
	const char *value = luaL_checklstring(L, 1, &length);
	bool accepted = false;
	luax_catchexcept(L, [&]() { accepted = instance()->setState(std::string(value, length)); });
	lua_pushboolean(L, accepted);
	return 1;
}

static const luaL_Reg functions[] = {{"start", w_start}, {"stop", w_stop}, {"isRunning", w_running},
	{"getPort", w_port}, {"poll", w_poll}, {"setState", w_setState}, {0, 0}};

extern "C" int luaopen_love_probe(lua_State *L)
{
	Probe *module = instance();
	if (module == nullptr)
		module = new Probe();
	else
		module->retain();

	WrappedModule wrapped;
	wrapped.module = module;
	wrapped.name = "probe";
	wrapped.type = &Module::type;
	wrapped.functions = functions;
	wrapped.types = nullptr;
	return luax_register_module(L, wrapped);
}

} // namespace probe
} // namespace love
