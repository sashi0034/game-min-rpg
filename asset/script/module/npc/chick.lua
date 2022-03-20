
ChickLuaData = {

    new = function()
        
        local self = Instantiate(ChickLuaData, IngameEventBase)
        
        self.vel = 50
        self.frameInterval = 200

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
        
        for i = 1, math.random(1, 5), 1 do
            x=self.getX(); y=self.getY();
            local r = GetRandomCrosswise16();
            if self.canMove(x + r.x, y + r.y) then
                local c = coroutine.create(self.doSleep)
                while self.doMove(x + r.x, y + r.y) do 
                    if not coroutine.resume(c, 3.0) then
                        ErrLog("Chick is stackted at "..x..", "..y.." and break loop.\n")
                        break
                    end
                    Yield() 
                end    
            end
        end

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 1.0) do Yield() end
    end,
}

