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

#include "Probe.h"

#include <lucent/http.h>

#include <deque>
#include <mutex>
#include <utility>

namespace love
{
namespace probe
{

struct Probe::State
{
	mutable std::mutex mutex;
	std::deque<Command> commands;
	std::string state_json;
	std::unique_ptr<lucent::http::Server> server;
	std::uint16_t port = 0;
};

Probe::Probe() : state(std::make_shared<State>()) {}

Probe::~Probe()
{
	stop();
}

bool Probe::start(std::uint16_t requested_port)
{
	std::lock_guard<std::mutex> lock(state->mutex);
	if (state->server)
		return false;

	lucent::http::ServerOptions options;
	options.port = requested_port;
	options.max_connections = 8;
	options.max_body_bytes = MAX_STATE_BYTES;
	std::weak_ptr<State> weak_state = state;
	std::unique_ptr<lucent::http::Server> server(new lucent::http::Server(options,
		[weak_state](const lucent::http::Request &request)
		{
			std::shared_ptr<State> current = weak_state.lock();
			if (!current)
				return lucent::http::Response::text(503, "Unavailable", "probe is stopping\n");
			if (request.method == "GET" && request.path() == "/state")
			{
				std::lock_guard<std::mutex> state_lock(current->mutex);
				return lucent::http::Response::json(
					200, "OK", current->state_json.empty() ? "{}" : current->state_json);
			}
			if (request.method != "POST" || request.path() != "/command")
				return lucent::http::Response::text(404, "Not Found", "unknown probe route\n");

			std::lock_guard<std::mutex> state_lock(current->mutex);
			if (current->commands.size() >= MAX_COMMANDS)
				return lucent::http::Response::text(429, "Too Many Requests", "probe queue is full\n");
			current->commands.push_back({request.method, request.target, request.body});
			return lucent::http::Response::text(202, "Accepted", "command queued\n");
		}));
	if (!server->start())
		return false;
	state->port = server->port();
	state->server = std::move(server);
	return true;
}

void Probe::stop()
{
	std::unique_ptr<lucent::http::Server> server;
	{
		std::lock_guard<std::mutex> lock(state->mutex);
		server = std::move(state->server);
		state->port = 0;
	}
}

bool Probe::running() const
{
	std::lock_guard<std::mutex> lock(state->mutex);
	return state->server && state->server->running();
}

std::uint16_t Probe::port() const
{
	std::lock_guard<std::mutex> lock(state->mutex);
	return state->port;
}

std::vector<Command> Probe::poll()
{
	std::lock_guard<std::mutex> lock(state->mutex);
	std::vector<Command> commands;
	commands.reserve(state->commands.size());
	while (!state->commands.empty())
	{
		commands.push_back(std::move(state->commands.front()));
		state->commands.pop_front();
	}
	return commands;
}

bool Probe::setState(const std::string &state_json_value)
{
	if (state_json_value.size() > MAX_STATE_BYTES)
		return false;
	std::lock_guard<std::mutex> lock(state->mutex);
	state->state_json = state_json_value;
	return true;
}

} // namespace probe
} // namespace love
