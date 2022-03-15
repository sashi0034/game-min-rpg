


cat_2 = {

    new = function()
        
        local self = Instantiate(cat_2, PunicatLuaData)
        
        self.events = {
            move = nil,
        }

        self.vel = 40

        OutLog("cat_2 is created.\n")

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
        local c, e1, e2, e3

        e1 = MapEventManager.getUnique("cat_2_move_1")
        
        e2 = MapEventManager.getUnique("cat_2_move_2")
        e3 = MapEventManager.getUnique("cat_2_move_3")
        
        while self.doMove(e1.x, e1.y) do Yield() end
        while self.doMove(e2.x, e2.y) do Yield() end
        while self.doMove(e3.x, e3.y) do Yield() end
        while self.doMove(e2.x, e2.y) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 0.5) do Yield() end

        while self.doMove(e1.x, e1.y) do Yield() end
    end,


    talk = function (self, e)
        local w = MessageWindow.open()

        w:streamText([[ã¥ÇÃè„Ç…Ç±ÇÌÇ¢ÉÇÉìÉXÉ^Å[Ç™Ç¢Çƒ]].."\n"..[[í ÇÍÇ»Ç¢Ç…Ç·]])
        while w:isRunning() do Yield() end

        w:streamText("\n"..[[Ç∑Ç≤Ç≠ç¢Ç¡ÇøÇ·Ç§Ç…Ç·]])
        while w:isRunning() do Yield() end

        w:close()
    end
}










