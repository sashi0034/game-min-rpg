
chick_4 = {

    new = function()
        
        local self = Instantiate(chick_4, ChickLuaData)
        
        self.events = {
            move = nil,
        }
        self.chickId = 4

        OutLog("chick_1 is created.\n")

        return self
    end,

    update = function(self)
        ChickLuaData.update(self)


        if (self.events.move==nil and self.events.followPlayer==nil) then
            self.events.move = coroutine.create(ChickLuaData.move)
            coroutine.resume(self.events.move, self)
        end

    end,


    talk = function (self, e)
        if self.events.followPlayer~=nil then
            return
        end

        local w = MessageWindow.open()

        self.awaits:sleep(0.5); Sound.dynamic("mew_chick.mp3")
        w:streamText("Ç“ÇÊÅAÇ“ÇÊ")
        while w:isRunning() do Yield() end

        w:streamText("\n"..[[Ç®Ç≥ÇÒÇ€äyÇµÇ¢]])
        while w:isRunning() do Yield() end

        self.events.followPlayer = coroutine.create(ChickLuaData.followPlayer)
        coroutine.resume(self.events.followPlayer, self)
        self.events.move = nil
        FlagManager.setFlag(FlagName.catch_chick_4, true)

        w:close()
    end
}




