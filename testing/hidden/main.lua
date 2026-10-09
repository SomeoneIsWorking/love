local output = assert(arg[#arg], "usage: love --hidden testing/hidden OUTPUT.png")
local frames = 0

function love.load()
    assert(love._hidden == true)
    assert(love.window.isOpen() and not love.window.isVisible(), "the window was mapped")
    assert(love.window.getVSync() == 0, "a hidden run kept vsync")
    love.window.setVSync(1)
    assert(love.window.getVSync() == 0, "setVSync re-enabled vsync in a hidden run")
    assert(love.window.showMessageBox("hidden fixture", "logged, not shown", "info") == false)
end

function love.draw()
    love.graphics.clear(1, 0, 0)
    frames = frames + 1
    if frames == 2 then
        love.graphics.captureScreenshot(function(image)
            local r, g, b = image:getPixel(0, 0)
            assert(r == 1 and g == 0 and b == 0, "the hidden backbuffer did not hold the frame")
            assert(io.open(output, "wb")):write(image:encode("png"):getString())
            assert(not love.window.isVisible(), "the window was mapped while presenting")
            love.event.quit(0)
        end)
    end
end
