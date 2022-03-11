MapEventManager:addMapEvents({

    house_1 = function (self, e)
        OutLog("Enter house.\n")
        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end
        OutLog("Ok.\n")
    end,

    house_2 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("これは家です！\nほげほげ")
        while w:isRunning() do Yield() end

        w:streamText("\n2a行目どうなるか")
        while w:isRunning() do Yield() end

        w:streamText("\n\nスクロールしてますね")
        while w:isRunning() do Yield() end

        w:close()

        w = MessageWindow.open()
        
        w:streamText([[ウインドウ再構築してみた]])
        while w:isRunning() do Yield() end

        w:streamText("\n\n\nやったぜ")
        while w:isRunning() do Yield() end

        w:close()
    end,

    
    stones_1 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("石ころが転がっている")

        
        local c = coroutine.create( self.doSleep )

        while w:isRunning() do Yield() end
        w:close()

        w = MessageWindow.open()
        w:streamText("眠い\nねむいです\nねます")
        while w:isRunning() do Yield() end

        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end

        w:close()
    end
})


