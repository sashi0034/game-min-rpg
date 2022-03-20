

chicken_by_fence = {

    new = function()
        
        local self = Instantiate(chicken_by_fence, PunicatLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 30

        return self
    end,

    update = function(self)
        ChickenLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(self.move)
            coroutine.resume(self.events.move, self)
        end

    end,

    move = function (self)
        for i = 1, 4, 1 do
            local e = MapEventManager.getUnique("chicken_by_fence_move_"..i)
            while self.doMove(e.x, e.y) do Yield() end
            if i==2 then
                local c = coroutine.create( self.doSleep )
                while coroutine.resume(c, 1.5) do Yield() end
            end
        end
    end,

    talk = function (self, e)
        local w = MessageWindow.open()

        w:streamText([[éqÇ«Ç‡ÇΩÇøÇ™ñ¿éqÇ…Ç»Ç¡ÇƒÇµÇ‹Ç¡Çƒ]].."\n"..[[ç¢Ç¡ÇΩÇÌ..]])
        while w:isRunning() do Yield() end

        if not FlagManager.getFlag(FlagName.pop_chick_1st) then
            FlagManager.setFlag(FlagName.pop_chick_1st, true)

            local e1 = MapEventManager.getUnique("chick_stray_1")
            MapEventManager.installCharacter(e1.x, e1.y, "chick", "chick_1")

            local e2 = MapEventManager.getUnique("chick_stray_2")
            MapEventManager.installCharacter(e2.x, e2.y, "chick", "chick_2")

            local e3 = MapEventManager.getUnique("chick_stray_3")
            MapEventManager.installCharacter(e3.x, e3.y, "chick", "chick_3")
        end

        w:close()
    end
    
}








