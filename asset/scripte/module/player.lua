

-- class Player
Player = {

    new = function()
        
        local self = Instantiate(Player, IngameEventBase)
        
        self.events = {
            move = nil,
        }
        self.vel = 50
        OutLog("Player in Lua is initilaized.\n")
        return self
    end,

    update = function(self)
        IngameEventBase.update(self)

        if (self.events.move==nil) then
            if (self.doWaitForMove() and self.isFixed()==false) then
                --OutLog("Player is prepareing for move.\n")
                self.events.move = coroutine.create(self.move)
                coroutine.resume(self.events.move, self)
            end
        end
    end,

    move = function (self)
        while self.doMove() do Yield() end
        --OutLog("Player move is completed.\n")
    end
}








