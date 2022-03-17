cat_goto_hill = {

    new = function()
        
        local self = Instantiate(cat_goto_hill, PunicatLuaData)
        
        self.events = {
            move = nil,
        }

        self.vel = 100
        self.frameInterval = 120

        OutLog("cat_goto_hill is created.\n")

        return self
    end,

    update = function(self)
        PunicatLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(self.move)
            coroutine.resume(self.events.move, self)
        end

    end,

    move = function (self)
        local c, e

        for i = 1, 9, 1 do
           e =  MapEventManager.getUnique("cat_goto_hill_move_"..i)
           while self.doMove(e.x, e.y) do Yield() end
        end

        self.setDeath(true)
        OutLog("cat_goto_hill is finished.")
    end,


    talk = function (self, e)
        local w = MessageWindow.open()

        w:streamText([[ÉjÉÉÇ…Ç·Ç…Ç·! ]])
        while w:isRunning() do Yield() end

        -- w:streamText([[Ç±Ç±Ç≈îLé®Ç≥ÇÒÇ∆]].."\n"..[[èoâÔÇ¡ÇΩÇÃÇÕâΩÇ©ÇÃâèÇ…Ç·]])
        -- while w:isRunning() do Yield() end

        w:close()
    end
}










