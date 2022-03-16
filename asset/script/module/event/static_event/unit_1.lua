MapEventManager:addStaticEvents({
    summonMouse = function (self)
        Yield()
        while not FlagManager.getFlag(FlagName.heard_mouse_rumor) do Yield() end
        OutLog("Mouse is ready to summon.\n")

        local c = coroutine.create(self.doSleep)
        while coroutine.resume(c, 2.0) do Yield() end

        local e = MapEventManager.getUnique("mouse_fast_start")

        MapEventManager.installCharacter(e.x, e.y, "mouse", "mouse_fast")
    end,
    -- sayHello = function (self)
    --     local c = coroutine.create(self.doSleep)
    --     while coroutine.resume(c, 3.0) do Yield() end

    --     local m = MessageWindow.open()

    --     m:streamText("‚¨‚Í‚æ‚¤‚²‚´‚¢‚Ü‚·")
    --     while m:isRunning() do Yield() end

    --     m:close()
    --     OutLog("Static event `sat Hello` is finished\n")
    -- end,
})

    