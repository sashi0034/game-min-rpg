
-- class TestNPC
TestNPC = {
    new = function(actor)
        local self = Instantiate(TestNPC)
        
        self.actor = actor
        self.time = 0
        self.cors = {
            move = COROUTINE_DEAD
        }
        
        return self
    end,

    
    update = function(self)
        for key, value in pairs(self.cors) do
            if (coroutine.status(value))~="dead" then
                coroutine.resume(value)
            end
        end


        if (coroutine.status(self.cors.move)~="dead") then
            coroutine.resume(self.cors.move)
        end
        
        if (coroutine.status(self.cors.move)=="dead" and self.time>200) then
            self.cors.move = coroutine.create( self.move )
            coroutine.resume(self.cors.move, self, self.getX(), self.getY())
        end
        self.time = self.time+Time.deltaMilli()
        return 0
    end,

    doWait = function (time)
        local t = 0
        while t<time do 
            t = t + Time.deltaSec()
            Yield(true)
        end
        return false
    end,

    move = function (self, x, y)
        local c

        while self.doMove(x+16, y+16) do Yield() end
        while self.doMove(x-16, y+16) do Yield() end

        c = coroutine.create( self.doWait )
        while coroutine.resume(c, 1.0) do Yield() end

        while self.doMove(x+16, y-16) do Yield() end
        while self.doMove(x-16, y-16) do Yield() end

        c = coroutine.create( self.doWait )
        while coroutine.resume(c, 2.0) do Yield() end

    end,


}








