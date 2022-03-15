


sheep_1 = {

    new = function()
        
        local self = Instantiate(sheep_1, PunicatLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 30

        return self
    end,

    update = function(self)
        PunicatLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(self.move)
            coroutine.resume(self.events.move, self)
        end

    end,

    move = function (self)
        local x, y, c

        local e1, e2, e3, e4
        e1 = MapEventManager.getUnique("sheep_1_move_1")
        e2 = MapEventManager.getUnique("sheep_1_move_2")
        e3 = MapEventManager.getUnique("sheep_1_move_3")
        e4 = MapEventManager.getUnique("sheep_1_move_4")

        while self.doMove(e1.x, e1.y) do Yield() end
        while self.doMove(e2.x, e2.y) do Yield() end
        while self.doMove(e3.x, e3.y) do Yield() end
        while self.doMove(e4.x, e4.y) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 1.5) do Yield() end

        while self.doMove(e3.x, e3.y) do Yield() end

        while self.doMove(e2.x, e2.y) do Yield() end
        while self.doMove(e1.x, e1.y) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end
    end,

    talk = function (self, e)
        local w = MessageWindow.open()

        w:streamText([[ÉÅÅ`ÅAÉÅÅ`]])
        while w:isRunning() do Yield() end

        w:streamText("\n"..[[ÇΩÇ≠Ç≥ÇÒëñÇ¡ÇƒåíçNÇ…Ç»ÇËÇ‹ÇµÇÂÇ§!]])
        while w:isRunning() do Yield() end

        w:close()
    end
    
}








