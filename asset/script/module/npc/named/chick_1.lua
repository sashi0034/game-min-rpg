
chick_1 = {

    new = function()
        
        local self = Instantiate(chick_1, ChickLuaData)
        
        self.events = {
            move = nil,
        }

        self.chickId = 1

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

        w:streamText("Ç…Ç·ÇÒÇ…Ç·ÇÒÅA")
        while w:isRunning() do Yield() end

        w:streamText([[Ç†ÅAä‘à·Ç¶ÇΩ]].."\n".."Ç“ÇÊÇ“ÇÊ")
        while w:isRunning() do Yield() end

        if not FlagManager.getFlag(FlagName.pop_chick_2nd) then
            FlagManager.setFlag(FlagName.pop_chick_2nd, true)
            local e4 = MapEventManager.getUnique("chick_stray_4")
            MapEventManager.installCharacter(e4.x, e4.y, "chick", "chick_4")

            local e5 = MapEventManager.getUnique("chick_stray_5")
            MapEventManager.installCharacter(e5.x, e5.y, "chick", "chick_5")
        end

        self.events.followPlayer = coroutine.create(ChickLuaData.followPlayer)
        coroutine.resume(self.events.followPlayer, self)
        self.events.move = nil
        FlagManager.setFlag(FlagName.catch_chick_1, true)

        w:close()
    end,
}




