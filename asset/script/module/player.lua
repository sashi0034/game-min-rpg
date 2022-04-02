

-- class Player
PlayerLuaData = {

    new = function()
        
        local self = Instantiate(PlayerLuaData, IngameEventBase)
        
        self.events = {
            move = nil,
        }
        self.vel = 50
        OutLog("Player in Lua is initilaized.\n")
        return self
    end,

    update = function(self)
        IngameEventBase.update(self)

        if (self.events.move==nil and self.events.killed==nil) then
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
    end,

    killed = function (self, e)
        self.awaits:sleep(1.0)
        e.blackFadeOut()
        self.awaits:sleep(2.0)

        local m = MessageWindow.open()
        self.awaits:streamText(m, e.message)
        self.awaits:sleep(0.5)
        e.exitScene()

        while true do
            Yield()
        end
    end
}








