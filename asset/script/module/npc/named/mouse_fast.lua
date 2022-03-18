

mouse_fast = {

    new = function()
        
        local self = Instantiate(mouse_fast, MouseLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 80
        self.frameInterval = 60

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
        local x, y, c

        local e1, e2, e3, e4
        e1 = MapEventManager.getUnique("mouse_fast_move_1")
        e2 = MapEventManager.getUnique("mouse_fast_move_2")
        e3 = MapEventManager.getUnique("mouse_fast_move_3")
        e4 = MapEventManager.getUnique("mouse_fast_move_4")

        while self.doMove(e1.x, e1.y) do Yield() end
        while self.doMove(e2.x, e2.y) do Yield() end
        while self.doMove(e3.x, e3.y) do Yield() end
        while self.doMove(e4.x, e4.y) do Yield() end

        self.setDeath(true)
    end,

    talk = function (self, e)
        local m = MessageWindow.open()

        if 
            FlagManager.getFlag(FlagName.obtain_super_dash) or
            FlagManager.getFlag(FlagName.friend_with_mouse) 
        then
            m:streamText([[ぼ、僕は悪くないっぴ!]])
            while m:isRunning() do Yield() end

            m:close()
            return
        end


        m:streamText([[ぼ、僕を食べないでくれっぴ!]])
        while m:isRunning() do Yield() end

        -- m:streamText("\n"..[[そうだ、君にもねずみの加護をつけて]].."\n"..[[あげるっぴ]])
        -- while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[見逃す]], [[食べる]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if (index==0) then
            local sleep

            m:streamText("\n"..[[ありがとうっぴ! ]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.friend_with_mouse, true)
        elseif index==1 then
            local sleep

            m:streamText("\n"..[[ひ、ひどいっぴ! ]])
            while m:isRunning() do Yield() end

            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:close()
            m = MessageWindow.open()

            m:streamText([[むしゃむしゃ、]])
            while m:isRunning() do Yield() end
            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:streamText([[もぐもぐ]])
            while m:isRunning() do Yield() end
            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:streamText("\n"..[[Shiftのダッシュがもっと速くなった!]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.obtain_super_dash, true)
            self.fadeAndDie()
        end

        m:close()
    end
    
}








