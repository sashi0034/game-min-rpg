
chick_5 = {

    new = function()
        
        local self = Instantiate(chick_5, ChickLuaData)
        
        self.events = {
            move = nil,
        }
        self.chickId = 5

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

        w:streamText("Ç“ÇÊÅAÇ“ÇÊ")
        while w:isRunning() do Yield() end

        local s = SelectionWindow.open({[[Ç“ÇÊ]], [[Ç…Ç·ÇÒ]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if index==0 then
            w:streamText("\n"..[[Ç“ÇÊ! Ç“ÇÊÇÊ..]])
            while w:isRunning() do Yield() end

            self.events.followPlayer = coroutine.create(ChickLuaData.followPlayer)
            coroutine.resume(self.events.followPlayer, self)
            self.events.move = nil

            FlagManager.setFlag(FlagName.catch_chick_5, true)
        elseif index==1 then
            w:streamText("\n"..[[Ç“ÇÊ?]])
            while w:isRunning() do Yield() end
        end

        w:close()
    end
}




