

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

        if (self.events.move==nil and 
            self.events.killed==nil and self.events.winning==nil) then
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
        Sound.dynamic("player_killed.mp3")
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
    end,

    winning = function (self, e)
        Sound.dynamic("player_winning.mp3")
        self.awaits:sleep(3.0)
        e.blackFadeOut()
        self.awaits:sleep(2.0)

        local m = MessageWindow.open()
        self.awaits:streamText(m, [[ñÇâ§ÇÕì|Ç≥ÇÍÇΩ]])
        self.awaits:streamText(m, "\n"..[[Ç±Ç§ÇµÇƒê¢äEÇÕã~ÇÌÇÍÇΩÇÃÇ≈Ç†Ç¡ÇΩ]])

        local p = math.floor(GetCompletedFlagRate() * 100)

        self.awaits:streamText(m, "\n"..[[ÉtÉâÉOäJï˙ó¶: ]]..p.." %")
        self.awaits:sleep(5.0)
        e.exitScene()

        while true do
            Yield()
        end
    end,

}








