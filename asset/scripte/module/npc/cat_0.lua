











cat_2 = {

    new = function()
        
        local self = Instantiate(cat_2, PunicatLuaData)
        
        self.events = {
            move = nil,
        }

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
        local x, y, c
        x=self.getX(); y=self.getY();

        while self.doMove(x-16, y) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 1.5) do Yield() end
        
        while self.doMove(x, y) do Yield() end
        while self.doMove(x+16, y) do Yield() end
        while self.doMove(x, y) do Yield() end
        while self.doMove(x, y-16) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 0.5) do Yield() end

        while self.doMove(x, y+16) do Yield() end

    end,


    talk = function (self, e)
        OutLog("aaaaa")

        local w = MessageWindow.open()

        w:streamText("Ç±ÇÒÇ…ÇøÇÕÇ…ÅAÇ±ÇÒÇ…ÇøÇÕ")
        while w:isRunning() do Yield() end
   
        w:close()
    end
}










