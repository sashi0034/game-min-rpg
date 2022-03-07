

-- class Player
Player = {

    new = function()
        
        local self = Instantiate(Player, IngameEventBase)
        
        self.events = {
            move = COROUTINE_DEAD,
        }
        self.vel = 50

        OutLog("Player in Lua is initilaized.\n")
        return self
    end,

    update = function(self)
        IngameEventBase.update(self)

        if (coroutine.status(self.events.move)=="dead") then
            if (self.doWaitForMove()) then
                OutLog("Player is prepareing for move.\n")
                self.events.move = coroutine.create(self.move)
                coroutine.resume(self.events.move, self)
            end
        end
    end,

    move = function (self)
        while self.doMove() do Yield() end
        OutLog("Player move is completed.\n")
    end
}








