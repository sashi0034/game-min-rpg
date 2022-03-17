MapEventManager:addStaticEvents({
    summonMouse = function (self)
        Yield()
        while not FlagManager.getFlag(FlagName.heard_mouse_rumor) do Yield() end
        OutLog("Mouse is ready to summon.\n")

        local c = coroutine.create(self.doSleep)
        while coroutine.resume(c, 12.0) do Yield() end

        local e = MapEventManager.getUnique("mouse_fast_start")

        MapEventManager.installCharacter(e.x, e.y, "mouse", "mouse_fast")
    end,
    walkerToHill = function (self)
        local c = coroutine.create(self.doSleep)
        while coroutine.resume(c, 30.0) do Yield() end

        OutLog("cat_goto_hill is ready to summon.\n")

        local e = MapEventManager.getUnique("cat_goto_hill_start")

        MapEventManager.installCharacter(e.x, e.y, "punicat", "cat_goto_hill")
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

    