


cat_1 = {

    new = function()
        
        local self = Instantiate(cat_1, PunicatLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 50

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
        x=self.getX(); y=self.getY();

        while self.doMove(x-16, y) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 1.5) do Yield() end
        
        while self.doMove(x, y) do Yield() end
        while self.doMove(x-32, y) do Yield() end
        while self.doMove(x-16, y) do Yield() end
        while self.doMove(x+16, y-16) do Yield() end
    end,

    talk = function (self, e)
        local w = MessageWindow.open()

        w:streamText("‚±‚ñ‚É‚¿‚Í‚±‚ñ‚É‚¿‚Í")
        while w:isRunning() do Yield() end
   
        w:close()
    end
    
}








