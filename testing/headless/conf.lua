function love.conf(config)
    -- The engine flag must take precedence over a game's requested modules.
    config.modules.window = true
    config.modules.graphics = true
    config.modules.audio = true
    config.modules.mouse = true
end
