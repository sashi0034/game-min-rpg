


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
        if FlagManager.getFlag(FlagName.kill_slime) then
            local c
            local e1 =  MapEventManager.getUnique("cat_2_move_a_1")
            local e2 =  MapEventManager.getUnique("cat_2_move_a_2")
            while self.doMove(e1.x, e1.y) do Yield() end
            
            local c = coroutine.create( self.doSleep )
            while coroutine.resume(c, 1.0) do Yield() end

            while self.doMove(e2.x, e2.y) do Yield() end

            c = coroutine.create( self.doSleep )
            while coroutine.resume(c, 1.0) do Yield() end
            return
        end

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

        if FlagManager.getFlag(FlagName.kill_slime) then
            w:streamText([[橋が通れるようになったにゃ]])
            while w:isRunning() do Yield() end

            w:streamText("\n"..[[おねえちゃん、ありがとうにゃ]])
            while w:isRunning() do Yield() end
        else
            w:streamText([[橋の上にこわいモンスターがいて]].."\n"..[[通れないにゃ]])
            while w:isRunning() do Yield() end
    
            w:streamText("\n"..[[すごく困っちゃうにゃ]])
            while w:isRunning() do Yield() end
        end

        w:close()
    end
}










