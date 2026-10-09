function love.conf(config)
    -- The engine flag must override a game that asks for vsync.
    config.window.vsync = 1
    config.window.width = 64
    config.window.height = 32
end
