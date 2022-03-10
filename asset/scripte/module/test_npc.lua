
-- class TestNPC
TestNPC = {
    new = function(actor)
        local self = Instantiate(TestNPC, IngameEventBase)
        
        self.actor = actor
        self.time = 0
        self.events = {
            move = nil
        }
        
        return self
    end,

    
    update = function(self)
        IngameEventBase.update(self)

        if (self.events.move==nil and self.time>200) then
            self.events.move = coroutine.create( self.move )
            coroutine.resume(self.events.move, self, self.getX(), self.getY())
        end

    end,

    move = function (self, x, y)
        local c

        while self.doMove(x+8, y+8) do Yield() end
        while self.doMove(x, y+8) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 1.0) do Yield() end

        while self.doMove(x+8, y-16) do Yield() end
        while self.doMove(x-24, y-16) do Yield() end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.0) do Yield() end

    end,


}








