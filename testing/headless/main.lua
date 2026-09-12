local updates = 0

function love.load(arguments)
    assert(love._headless == true)
    assert(arguments[1] == "--fixture-argument")
    assert(love.window == nil and love.graphics == nil and love.audio == nil)
    assert(love.mouse == nil and love.keyboard == nil and love.joystick == nil)
    assert(love.filesystem and love.data and love.event and love.timer)
end

function love.update(delta_seconds)
    assert(type(delta_seconds) == "number" and delta_seconds >= 0)
    updates = updates + 1
    if updates == 3 then
        love.event.quit(0)
    end
end

function love.draw()
    error("headless mode must not call love.draw")
end
