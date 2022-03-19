
ChickLuaData = {

    new = function()
        
        local self = Instantiate(ChickLuaData, IngameEventBase)
        
        self.vel = 50
        self.frameInterval = 100

        return self
    end,

    update = function(self)
        IngameEventBase.update(self)

        if (self.events.talk~=nil) then
            self.pausedEvents.move = true
        else
            self.pausedEvents.move = nil
        end
    end,

    move = function (self)
        local x, y, c
        x=self.getX(); y=self.getY();

        while self.doMove(x-16, y) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 1.0) do Yield() end
        
        while self.doMove(x, y) do Yield() end
        while self.doMove(x+16, y) do Yield() end
        while self.doMove(x, y) do Yield() end
        while self.doMove(x, y-16) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 1.0) do Yield() end

        while self.doMove(x, y+16) do Yield() end
    end,
}

